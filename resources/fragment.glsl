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

const int WALKING_TREE_STACK_MAX_SIZE = 256;
int WALKING_TREE_STACK_CURRENT_SIZE = 0;
int WALKING_TREE_STACK[WALKING_TREE_STACK_MAX_SIZE];

const int COMPUTING_TREE_STACK_MAX_SIZE = 8; // Вероятно, можно спокойно поставить максимальный размер в 2 или 4.
int COMPUTING_TREE_STACK_CURRENT_SIZE = 0;
float COMPUTING_TREE_STACK[COMPUTING_TREE_STACK_MAX_SIZE];

#define pushWalkingSceneStack(data) WALKING_TREE_STACK[WALKING_TREE_STACK_CURRENT_SIZE++] = data
#define popWalkingSceneStack() WALKING_TREE_STACK[--WALKING_TREE_STACK_CURRENT_SIZE]
#define peekWalkingSceneStack() WALKING_TREE_STACK[WALKING_TREE_STACK_CURRENT_SIZE - 1]
#define isWalkingSceneStackEmpty() (WALKING_TREE_STACK_CURRENT_SIZE == 0)

#define pushComputingSceneStack(data) COMPUTING_TREE_STACK[COMPUTING_TREE_STACK_CURRENT_SIZE++] = data
#define popComputingSceneStack() COMPUTING_TREE_STACK[--COMPUTING_TREE_STACK_CURRENT_SIZE]
#define peekComputingSceneStack() COMPUTING_TREE_STACK[COMPUTING_TREE_STACK_CURRENT_SIZE - 1]
#define isComputingSceneStackEmpty() (COMPUTING_TREE_STACK_CURRENT_SIZE == 0)

struct SceneNode {
    int left;
    int right;

    int operation;

    // Костыль из-за выравнивания vec3 до vec4
    float localPositionX;
    float localPositionY;
    float localPositionZ;

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

layout(std430, binding = 2) buffer Scene {
    SceneNode[] nodes;
};

/* - = - Materials - = - */

struct ObjectMaterial {
    vec3 color;
    vec3 specColor;
    float shininess;
};

ObjectMaterial MATERIALS[6] = ObjectMaterial[](
    ObjectMaterial(vec3(0.5), vec3(1), 2),
    ObjectMaterial(vec3(0, 1, 0), vec3(1), 16),
    ObjectMaterial(vec3(0, 0, 1), vec3(1), 32),
    ObjectMaterial(vec3(1, 1, 0), vec3(1), 2),
    ObjectMaterial(vec3(1, 0, 1), vec3(1), 2),
    ObjectMaterial(vec3(1, 1, 1), vec3(1), 2)
);

/* - = - SDF - = - */

struct SceneObject {
    int materialId;
    float distance;
};

// Источник SDF-функций https://iquilezles.org/articles/distfunctions/

float sphereSdf(in vec3 p, in float r) {
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

SceneObject mergeOp(in SceneObject a, in SceneObject b) {
    return a.distance < b.distance ? a : b;
}

SceneObject intersectionOp(in SceneObject a, in SceneObject b) {
    return a.distance < b.distance ? b : a;
}

SceneObject subtractionOp(in SceneObject a, in SceneObject b) {
    return -a.distance < b.distance ? b : SceneObject(a.materialId, -a.distance);
}

/* - = - Scene processing - = - */

float processLeafSceneNode(in vec3 point, int nodeIndex) {
    int figureType = nodes[nodeIndex].figureType;

    if (FIGURE_SPHERE == figureType) {
        float radius = nodes[nodeIndex].figureVariable1;
        return sphereSdf(point, nodes[nodeIndex].figureVariable1);
    }

    if (FIGURE_BOX == figureType) {
        float xSize = nodes[nodeIndex].figureVariable1;
        float ySize = nodes[nodeIndex].figureVariable2;
        float zSize = nodes[nodeIndex].figureVariable3;

        return boxSdf(point, vec3(xSize, ySize, zSize));
    }

    if (FIGURE_TORUS == figureType) {
        float smallRadius = nodes[nodeIndex].figureVariable1;
        float largeRadius = nodes[nodeIndex].figureVariable2;

        return torusSdf(point, smallRadius, largeRadius);
    }

    if (FIGURE_PLANE == figureType) {
        float xNormal = nodes[nodeIndex].figureVariable4;
        float yNormal = nodes[nodeIndex].figureVariable5;
        float zNormal = nodes[nodeIndex].figureVariable6;

        float distanceFromOrigin = nodes[nodeIndex].figureVariable7;

        return planeSdf(point, vec3(xNormal, yNormal, zNormal), distanceFromOrigin);
    }

    if (FIGURE_CYLINDER == figureType) {
        float radius = nodes[nodeIndex].figureVariable1;
        float height = nodes[nodeIndex].figureVariable2;

        return cylinderSdf(point, height, radius);
    }

    return MAX_DISTANCE;
}

float processSceneNode(in vec3 point, int nodeIndex) {
    int nodeOperation = nodes[nodeIndex].operation;

    if (OPERATION_LEAF == nodeOperation) {
        return processLeafSceneNode(point, nodeIndex);
    }

    float left = popComputingSceneStack();
    float right = popComputingSceneStack();

    if (OPERATION_SUBSTRACTION == nodeOperation) {
        return subtractionOp(SceneObject(1, left), SceneObject(2, right)).distance;
    }

    if (OPERATION_MERGE == nodeOperation) {
        return mergeOp(SceneObject(1, left), SceneObject(2, right)).distance;
    }

    if (OPERATION_INTERSECTION == nodeOperation) {
        return intersectionOp(SceneObject(1, left), SceneObject(2, right)).distance;
    }

    return MAX_DISTANCE;
}

float processSceneTree(in vec3 point, int rootIndex) {
    int node = rootIndex;

    while (node != NULL_NODE_INDEX || !isWalkingSceneStackEmpty()) {
        if (!isWalkingSceneStackEmpty()) {
            node = popWalkingSceneStack();

            if (!isWalkingSceneStackEmpty() && nodes[node].right == peekWalkingSceneStack()) {
                node = popWalkingSceneStack();
            } else {
                float nodeDistance = processSceneNode(point, node);
                pushComputingSceneStack(nodeDistance);
                node = NULL_NODE_INDEX;
            }
        }

        while (node != NULL_NODE_INDEX) {
            pushWalkingSceneStack(node);
            if (nodes[node].right != NULL_NODE_INDEX) {
                pushWalkingSceneStack(nodes[node].right);
                pushWalkingSceneStack(node);
            }

            node = nodes[node].left;
        }
    }

    return popComputingSceneStack();
}

void map(in vec3 point, out SceneObject sceneObject) {
    sceneObject.distance = processSceneTree(point, 0);
    sceneObject.materialId = 2;
}

SceneObject map(in vec3 point) {
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
bool rayMarching(in vec3 rayOrigin, in vec3 rayDirection, out SceneObject sceneObject) {
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
float shadowMarching(in vec3 lightPosition, in vec3 point) {
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
vec3 estimateNormal(in vec3 point) {
    float sceneDist = map(point).distance;
	return normalize(vec3(
        map(vec3(point.x + EPSILON, point.y, point.z)).distance - sceneDist,
        map(vec3(point.x, point.y + EPSILON, point.z)).distance - sceneDist,
        map(vec3(point.x, point.y, point.z + EPSILON)).distance - sceneDist));
}

/*
 * Вычисление направления луча для одного пикселя.
*/
vec3 computeRayDirection(in float fov, in vec2 resolution, in vec2 fragCoord){
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

Light createPointLight(in vec3 color, in vec3 position, float ambientStrength, LightAttenuation attenuation) {
    return Light(color, position, vec3(0), ambientStrength, attenuation);
}

Light createSkyLight(in vec3 color, in vec3 direction, float ambientStrength) {
    return Light(color, vec3(0), direction, ambientStrength, LightAttenuation(0.0, 0.0, 0.0));
}

vec3 computeLightAmbient(in Light light, in ObjectMaterial material) {
    return light.ambientStrength * light.color * material.color;
}

vec3 computeLightDiffuse(in Light light, in ObjectMaterial material, in vec3 lightNormal, in vec3 objectNormal) {
    float dotLN = max(0.0, dot(objectNormal, lightNormal));
    return dotLN * light.color * material.color;
}

vec3 computeLightSpecular(in Light light, in ObjectMaterial material, in vec3 lightNormal, in vec3 objectNormal, in vec3 eyeVector) {
    vec3 reflection = normalize(reflect(-lightNormal, objectNormal));
    float dotRV = max(0.0, dot(eyeVector, reflection));
    float specularStrength = pow(dotRV, material.shininess);
    return specularStrength * material.specColor * light.color;
}

/**
 * Вычисляет коэф. затухания света с ростом расстоияния от источника освещения по
 * формуле затухания.
 * Fatt = 1.0 / (Kc + Kl*d + Kq*d*d)
 */
float computeLightAttenuation(in Light light, in vec3 point) {
    float distanceToLight = length(light.position - point);
    LightAttenuation attenuation = light.attenuation;
    return 1.0 / (attenuation.constant + attenuation.linear * distanceToLight +
        attenuation.quadratic * (distanceToLight * distanceToLight));
}

vec3 computePointLight(in Light light, in ObjectMaterial material, in vec3 point, in vec3 normal, in vec3 eyeVector) {
    vec3 lightNormal = normalize(light.position - point);
    float fade = computeLightAttenuation(light, point);

    vec3 ambient = computeLightAmbient(light, material) * fade;
    vec3 diffuse = computeLightDiffuse(light, material, lightNormal, normal) * fade;
    vec3 specular = computeLightSpecular(light, material, lightNormal, normal, eyeVector) * fade;

    float shadow = shadowMarching(light.position, point);

    return ambient + (diffuse + specular) * shadow;
}

vec3 computeSkyLight(in Light light, in ObjectMaterial material, in vec3 point, in vec3 normal, in vec3 eyeVector) {
    vec3 diractionToLight = -light.direction;
    vec3 lightNormal = normalize(diractionToLight);

    vec3 ambient = computeLightAmbient(light, material);
    vec3 diffuse = computeLightDiffuse(light, material, lightNormal, normal);
    vec3 specular = computeLightSpecular(light, material, lightNormal, normal, eyeVector);

    float shadow = shadowMarching(point + MAX_DISTANCE * diractionToLight, point);

    return ambient + (diffuse + specular) * shadow;
}

/* - = - Main - = - */

void renderImage(inout vec3 pixelColor, in vec4 gl_FragCoord) {
    vec3 eyePosition = u_cameraPosition;
    vec3 rayDirection = computeRayDirection(FOV, vec2(u_resolution.xy), gl_FragCoord.xy);
    vec3 viewDirection = (u_view * vec4(rayDirection, 0.0)).xyz;

    SceneObject nearestObject = SceneObject(0, 0.0);
    bool isObjectHitted = rayMarching(eyePosition, viewDirection, nearestObject);

    if (!isObjectHitted) {
        return;
    }

    vec3 intersectionPosition = eyePosition + nearestObject.distance * viewDirection;
    vec3 normal = estimateNormal(intersectionPosition);
    vec3 eyeVector = normalize(eyePosition - intersectionPosition);

    Light light = createSkyLight(vec3(1), -vec3(0.57735, 0.57735, 0.57735), 0.2);
    ObjectMaterial material = MATERIALS[nearestObject.materialId];

    pixelColor += computeSkyLight(light, material, intersectionPosition, normal, eyeVector);
}

void main() {
    vec3 pixelColor = vec3(0.0);
    renderImage(pixelColor, gl_FragCoord);
    fs_color = vec4(pixelColor, 1.0);
}
