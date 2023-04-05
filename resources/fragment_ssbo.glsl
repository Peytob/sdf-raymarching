#pragma include "fragment.glsl"

/* - = - Options - = - */

const int COMPUTING_TREE_STACK_MAX_SIZE = 16; // Вероятно, можно спокойно поставить максимальный размер в 2 или 4.
const int WALKING_TREE_STACK_MAX_SIZE = 256;
const int WALKING_TREE_MAX_DEPTH = 24; // Защита от зацикливания при неверном вводе

const int FIGURE_SPHERE = 1;
const int FIGURE_BOX = 2;
const int FIGURE_TORUS = 3;
const int FIGURE_PLANE = 4;
const int FIGURE_CYLINDER = 5;

const int NULL_NODE_INDEX = -1;
const int OPERATION_LEAF = 0;
const int OPERATION_SUBSTRACTION = 1;
const int OPERATION_MERGE = 2;
const int OPERATION_INTERSECTION = 3;

/* - = - Data types - = - */

struct SceneNode {
    int left;
    int right;

    int operation;

// Костыль из-за выравнивания vec3 до vec4
    float localPositionX;
    float localPositionY;
    float localPositionZ;

    int materialId;
    int figureType;

// TODO Найти способ реализации union или его подобия в glsl!

    float figureVariable1;
    float figureVariable2;
    float figureVariable3;
    float figureVariable4;
    float figureVariable5;
    float figureVariable6;
    float figureVariable7;
    float figureVariable8;
    float figureVariable9;
    float figureVariable10;
    float figureVariable11;
    float figureVariable12;
};

/* - = - Buffers - = - */

layout(std430, binding = 1) buffer Scene {
    SceneNode[] nodes;
};

/* Stack util */

int WALKING_TREE_STACK_CURRENT_SIZE = 0;
int WALKING_TREE_STACK[WALKING_TREE_STACK_MAX_SIZE];

int COMPUTING_TREE_STACK_CURRENT_SIZE = 0;
SceneObject COMPUTING_TREE_STACK[COMPUTING_TREE_STACK_MAX_SIZE];

#define pushWalkingSceneStack(data) WALKING_TREE_STACK[WALKING_TREE_STACK_CURRENT_SIZE++] = data
#define popWalkingSceneStack() WALKING_TREE_STACK[--WALKING_TREE_STACK_CURRENT_SIZE]
#define peekWalkingSceneStack() WALKING_TREE_STACK[WALKING_TREE_STACK_CURRENT_SIZE - 1]
#define isWalkingSceneStackEmpty() (WALKING_TREE_STACK_CURRENT_SIZE == 0)

#define pushComputingSceneStack(data) COMPUTING_TREE_STACK[COMPUTING_TREE_STACK_CURRENT_SIZE++] = data
#define popComputingSceneStack() COMPUTING_TREE_STACK[--COMPUTING_TREE_STACK_CURRENT_SIZE]
#define peekComputingSceneStack() COMPUTING_TREE_STACK[COMPUTING_TREE_STACK_CURRENT_SIZE - 1]
#define isComputingSceneStackEmpty() (COMPUTING_TREE_STACK_CURRENT_SIZE == 0)

/* - = - Scene processing - = - */

float processLeafSceneNodeDistance(vec3 point, int nodeIndex) {
    int figureType = nodes[nodeIndex].figureType;

    point -= vec3(
        nodes[nodeIndex].localPositionX,
        nodes[nodeIndex].localPositionY,
        nodes[nodeIndex].localPositionZ
    );

    if (FIGURE_SPHERE == figureType) {
        float radius = nodes[nodeIndex].figureVariable1;
        return sphereSdf(point, nodes[nodeIndex].figureVariable1);
    }

    else if (FIGURE_BOX == figureType) {
        float xSize = nodes[nodeIndex].figureVariable1;
        float ySize = nodes[nodeIndex].figureVariable2;
        float zSize = nodes[nodeIndex].figureVariable3;

        return boxSdf(point, vec3(xSize, ySize, zSize));
    }

    else if (FIGURE_TORUS == figureType) {
        float smallRadius = nodes[nodeIndex].figureVariable1;
        float largeRadius = nodes[nodeIndex].figureVariable2;

        return torusSdf(point, smallRadius, largeRadius);
    }

    else if (FIGURE_PLANE == figureType) {
        float xNormal = nodes[nodeIndex].figureVariable1;
        float yNormal = nodes[nodeIndex].figureVariable2;
        float zNormal = nodes[nodeIndex].figureVariable3;

        float distanceFromOrigin = nodes[nodeIndex].figureVariable4;

        return planeSdf(point, vec3(xNormal, yNormal, zNormal), distanceFromOrigin);
    }

    else if (FIGURE_CYLINDER == figureType) {
        float radius = nodes[nodeIndex].figureVariable1;
        float height = nodes[nodeIndex].figureVariable2;

        return cylinderSdf(point, height, radius);
    }

    return MAX_DISTANCE;
}

SceneObject processLeafSceneNode(vec3 point, int nodeIndex) {
    float nodeDistance = processLeafSceneNodeDistance(point, nodeIndex);
    return SceneObject(nodes[nodeIndex].materialId, nodeDistance);
}

SceneObject processSceneNode(vec3 point, int nodeIndex) {
    int nodeOperation = nodes[nodeIndex].operation;

    if (OPERATION_LEAF == nodeOperation) {
        return processLeafSceneNode(point, nodeIndex);
    }

    SceneObject left = popComputingSceneStack();
    SceneObject right = popComputingSceneStack();

    if (OPERATION_SUBSTRACTION == nodeOperation) {
        return subtractionOp(left, right);
    }

    else if (OPERATION_MERGE == nodeOperation) {
        return mergeOp(left, right);
    }

    else if (OPERATION_INTERSECTION == nodeOperation) {
        return intersectionOp(left, right);
    }

    return SceneObject(0, MAX_DISTANCE);
}

SceneObject processSceneTree(vec3 point, int rootIndex) {
    int node = rootIndex;
    int outerWhileDepth = 0;

    while (outerWhileDepth < WALKING_TREE_MAX_DEPTH && node != NULL_NODE_INDEX || !isWalkingSceneStackEmpty()) {
        if (!isWalkingSceneStackEmpty()) {
            node = popWalkingSceneStack();

            if (!isWalkingSceneStackEmpty() && nodes[node].right == peekWalkingSceneStack()) {
                node = popWalkingSceneStack();
            } else {
                SceneObject nodeDistance = processSceneNode(point, node);
                pushComputingSceneStack(nodeDistance);
                node = NULL_NODE_INDEX;
            }
        }

        int innerWhileDepth = 0;

        while (innerWhileDepth < WALKING_TREE_MAX_DEPTH && node != NULL_NODE_INDEX) {
            pushWalkingSceneStack(node);
            if (nodes[node].right != NULL_NODE_INDEX) {
                pushWalkingSceneStack(nodes[node].right);
                pushWalkingSceneStack(node);
            }

            node = nodes[node].left;
            innerWhileDepth++;
        }

        outerWhileDepth++;
    }

    return popComputingSceneStack();
}

void map(vec3 point, out SceneObject sceneObject) {
    sceneObject = processSceneTree(point, 0);
}
