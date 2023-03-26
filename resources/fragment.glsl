#version 430 core

out vec4 fs_color;

uniform ivec2 u_resolution;
uniform mat4 u_view;
uniform vec3 u_cameraPosition;

const float FOV = 90; // TODO Получать из u_view
const int MAX_STEPS = 255;
const float MIN_DISTANCE = 0.0;
const float MAX_DISTANCE = 100.0;
const float EPSILON = 0.0001;

const int COMPUTING_TREE_STACK_MAX_SIZE = 16; // Вероятно, можно спокойно поставить максимальный размер в 2 или 4.
const int WALKING_TREE_STACK_MAX_SIZE = 256;
const int WALKING_TREE_MAX_DEPTH = 24; // Защита от зацикливания при неверном вводе

/* - = - Data types - = - */

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

/* - = - Scene node data - = - */

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

/* - = - Materials - = - */

struct ObjectMaterial {
    vec4 color;
    vec4 specColor;
    vec4 shininess;
};

/* - = - Buffers - = - */

layout(std430, binding = 1) buffer Scene {
    SceneNode[] nodes;
};

layout(std430, binding = 2) buffer Materials {
    ObjectMaterial[] materials;
};

/* - = - SDF - = - */

struct SceneObject {
    int materialId;
    float distance;
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

// Источник SDF-функций https://iquilezles.org/articles/distfunctions/

float sphereSdf(vec3 p, float r) {
    return length(p) - r;
}

float boxSdf(vec3 p, vec3 b) {
    vec3 q = abs(p) - b;
    return length(max(q, 0.0)) + min(max(q.x, max(q.y,q.z)), 0.0);
}

float torusSdf(vec3 p, float smallRadius, float largeRadius) {
    return length(vec2(length(p.xz) - largeRadius, p.y)) - smallRadius;
}

float planeSdf(vec3 p, vec3 n, float distanceFromOrigin) {
    return dot(p, n) + distanceFromOrigin;
}

float cylinderSdf(vec3 p, float h,float r) {
    vec2 d = abs(vec2(length(p.xz),p.y)) - vec2(r,h);
    return min(max(d.x,d.y),0.0) + length(max(d,0.0));
}

SceneObject mergeOp(SceneObject a, SceneObject b) {
    return a.distance < b.distance ? a : b;
}

SceneObject intersectionOp(SceneObject a, SceneObject b) {
    return a.distance < b.distance ? b : a;
}

SceneObject subtractionOp(SceneObject a, SceneObject b) {
    return -a.distance < b.distance ? b : SceneObject(a.materialId, -a.distance);
}

/* - = - Scene processing - = - */

float processLeafSceneNodeDistance(vec3 point, int nodeIndex) {
    int figureType = nodes[nodeIndex].figureType;

    point += vec3(
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

SceneObject map(vec3 point) {
    SceneObject sceneObject;
    map(point, sceneObject);
    return sceneObject;
}

/* - = - Ray Marching - = - */

/*
 * Возвращает true, если было пересечение с каким-либо объектом и записывает данные
 * о пересечении в sceneObject. Возвращает false, если пересечений нет, при этом
 * sceneObject остается неизменным.
*/
bool rayMarching(vec3 rayOrigin, vec3 rayDirection, out SceneObject sceneObject) {
    float rayDistance = 0.0;
    SceneObject mappedObject = SceneObject(0, 0.0);

    for (int i = 0; i < MAX_STEPS; ++i) {
        vec3 currentPoint = rayOrigin + rayDistance * rayDirection;
        map(currentPoint, mappedObject);

        if (abs(mappedObject.distance) < EPSILON) {
            break;
        }

        rayDistance += mappedObject.distance;

        if (rayDistance > MAX_DISTANCE) {
            return false;
        }
    }

    sceneObject.distance = rayDistance;
    sceneObject.materialId = mappedObject.materialId;

    return true;
}

/**
 * Облегченная версия rayMarching с небольшими дополнительными смещениями для луча.
 * Выполняет меньше операций, относительно аналогичного вызова rayMarching и не возвращает
 * лишних данных
 */
float shadowMarching(vec3 lightPosition, vec3 point) {
    vec3 rayDirection = normalize(lightPosition - point);
    vec3 rayOrigin = point;

    float maxRayLength = length(lightPosition - point);
    float rayDistance = 0.02; // Коэф. смещения для луча, чтобы не врезался в место своего старта

    for (int i = 0; i < MAX_STEPS; ++i) {
        vec3 currentPoint = point + rayDistance * rayDirection;
        float currentNearestObjectDistance = map(currentPoint).distance;

        if (currentNearestObjectDistance < EPSILON) {
            return 0.0;
        }

        rayDistance += currentNearestObjectDistance;

        if (rayDistance > maxRayLength) {
            break;
        }
    }

    return 1.0;
}

/**
 * Вычисляет примерную нормаль в точке, исходя из градиента вокруг нее
*/
vec3 estimateNormal(vec3 point) {
    float sceneDist = map(point).distance;
    return normalize(vec3(
    map(vec3(point.x + EPSILON, point.y, point.z)).distance - sceneDist,
    map(vec3(point.x, point.y + EPSILON, point.z)).distance - sceneDist,
    map(vec3(point.x, point.y, point.z + EPSILON)).distance - sceneDist));
}

/*
 * Вычисление направления луча для одного пикселя.
*/
vec3 computeRayDirection(float fov, vec2 resolution, vec2 fragCoord){
    vec2 xy = fragCoord - resolution * 0.5;
    float z = resolution.y / tan(radians(fov) * 0.5);
    return normalize(vec3(xy, -z));
}

/* - = - Lighting - = - */

struct LightAttenuation {
    float constant;
    float linear;
    float quadratic;
};

struct Light {
    vec3 color;
    vec3 position;
    vec3 direction;
    float ambientStrength;

    LightAttenuation attenuation;
};

Light createPointLight(vec3 color, vec3 position, float ambientStrength, LightAttenuation attenuation) {
    return Light(color, position, vec3(0), ambientStrength, attenuation);
}

Light createSkyLight(vec3 color, vec3 direction, float ambientStrength) {
    return Light(color, vec3(0), direction, ambientStrength, LightAttenuation(0.0, 0.0, 0.0));
}

vec3 computeLightAmbient(Light light, ObjectMaterial material) {
    return light.ambientStrength * light.color * material.color.xyz;
}

vec3 computeLightDiffuse(Light light, ObjectMaterial material, vec3 lightNormal, vec3 objectNormal) {
    float dotLN = max(0.0, dot(objectNormal, lightNormal));
    return dotLN * light.color * material.color.xyz;
}

vec3 computeLightSpecular(Light light, ObjectMaterial material, vec3 lightNormal, vec3 objectNormal, vec3 eyeVector) {
    vec3 reflection = normalize(reflect(-lightNormal, objectNormal));
    float dotRV = max(0.0, dot(eyeVector, reflection));
    float specularStrength = pow(dotRV, material.shininess.x);
    return specularStrength * material.specColor.xyz * light.color;
}

/**
 * Вычисляет коэф. затухания света с ростом расстоияния от источника освещения по
 * формуле затухания.
 * Fatt = 1.0 / (Kc + Kl*d + Kq*d*d)
 */
float computeLightAttenuation(Light light, vec3 point) {
    float distanceToLight = length(light.position - point);
    LightAttenuation attenuation = light.attenuation;
    return 1.0 / (attenuation.constant + attenuation.linear * distanceToLight +
    attenuation.quadratic * (distanceToLight * distanceToLight));
}

vec3 computePointLight(Light light, ObjectMaterial material, vec3 point, vec3 normal, vec3 eyeVector) {
    vec3 lightNormal = normalize(light.position - point);
    float fade = computeLightAttenuation(light, point);

    vec3 ambient = computeLightAmbient(light, material) * fade;
    vec3 diffuse = computeLightDiffuse(light, material, lightNormal, normal) * fade;
    vec3 specular = computeLightSpecular(light, material, lightNormal, normal, eyeVector) * fade;

    float shadow = shadowMarching(light.position, point);

    return ambient + (diffuse + specular) * shadow;
}

vec3 computeSkyLight(Light light, ObjectMaterial material, vec3 point, vec3 normal, vec3 eyeVector) {
    vec3 diractionToLight = -light.direction;
    vec3 lightNormal = normalize(diractionToLight);

    vec3 ambient = computeLightAmbient(light, material);
    vec3 diffuse = computeLightDiffuse(light, material, lightNormal, normal);
    vec3 specular = computeLightSpecular(light, material, lightNormal, normal, eyeVector);

    float shadow = shadowMarching(point + MAX_DISTANCE * diractionToLight, point);

    return ambient + (diffuse + specular) * shadow;
}

/* - = - Main - = - */

void renderImage(inout vec3 pixelColor, vec4 gl_FragCoord) {
    vec3 eyePosition = u_cameraPosition;
    vec3 rayDirection = computeRayDirection(FOV, vec2(u_resolution.xy), gl_FragCoord.xy);
    vec3 viewDirection = (u_view * vec4(rayDirection, 0.0)).xyz;

    SceneObject nearestObject = SceneObject(0, 0.0);
    bool isObjectHitted = rayMarching(eyePosition, viewDirection, nearestObject);

    if (!isObjectHitted) {
        pixelColor += vec3(0.1, 0.1, 0.1);
        return;
    }

    vec3 intersectionPosition = eyePosition + nearestObject.distance * viewDirection;
    vec3 normal = estimateNormal(intersectionPosition);
    vec3 eyeVector = normalize(eyePosition - intersectionPosition);

    Light light = createSkyLight(vec3(1), -vec3(0.57735, 0.57735, 0.57735), 0.2);
    ObjectMaterial material = materials[nearestObject.materialId];

    pixelColor += computeSkyLight(light, material, intersectionPosition, normal, eyeVector);
}

void main() {
    vec3 pixelColor = vec3(0.0);
    renderImage(pixelColor, gl_FragCoord);
    fs_color = vec4(pixelColor, 1.0);
}
