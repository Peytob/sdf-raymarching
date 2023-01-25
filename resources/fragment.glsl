#version 330 core

out vec4 fs_color;

uniform ivec2 u_resolution;
uniform mat4 u_view;
uniform vec3 u_cameraPosition;

const float FOV = 90; // TODO Получать из u_view
const int MAX_STEPS = 255;
const float MIN_DISTANCE = 0.0;
const float MAX_DISTANCE = 100.0;
const float EPSILON = 0.0001;

/* - = - Noise - = - */

float rand(vec2 co){
    return fract(sin(dot(co, vec2(12.9898, 78.233))) * 43758.5453);
}

/*
 * a +---+ b
 *   |   |
 * c +---+ d
 * Псевдо-рандомные высоты точек a, b, c, d на сетке и последующая
 * интерполяция к точке f (дробная часть точки в сетке 1x1)
*/
float simpleNoice(vec2 point) {
    vec2 i = floor(point);
    vec2 f = fract(point);

    float a = rand(i);
    float b = rand(i + vec2(1.0, 0.0));
    float c = rand(i + vec2(0.0, 1.0));
    float d = rand(i + vec2(1.0, 1.0));

    vec2 fade = smoothstep(0.0, 1.0, f);

    return
        mix(a, b, fade.x) +
        (c - a) * fade.y * (1.0 - fade.x) +
        (d - b) * fade.x * fade.y;
}

float noise2d(vec2 point) {
    return simpleNoice(point);
}

/* - = - SDF - = - */

struct SceneObject {
    int id;
    float distance;
};

float sphereSdf(in vec3 p, in float r) {
    return length(p) - r;
}

float torusSdf(vec3 p, float smallRadius, float largeRadius) {
	return length(vec2(length(p.xz) - largeRadius, p.y)) - smallRadius;
}

float planeSdf(vec3 p, vec3 n, float distanceFromOrigin) {
	return dot(p, n) + distanceFromOrigin;
}

float unionOp(in float a, in float b) {
    return min(a, b);
}

void map(in vec3 point, out SceneObject sceneObject) {
    sceneObject.distance = unionOp(
        torusSdf(point, 3, 5),
        unionOp(
            sphereSdf(point + vec3(-4, 10, 4), 3.0),
            unionOp(
                sphereSdf(point + vec3(30, 0, 4), 3.0),
                planeSdf(point, vec3(0, 1, 0), 20)
            )
        )
    );
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

struct ObjectMaterial {
    vec3 color;
    vec3 specColor;
    float shininess;
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

    Light light = createSkyLight(vec3(1, 1, 1), -vec3(0.57735, 0.57735, 0.57735), 0.2);
    ObjectMaterial material = ObjectMaterial(vec3(0.5, 0.9, 0.9), vec3(1.0) * 0.5, 16.0);

    pixelColor += computeSkyLight(light, material, intersectionPosition, normal, eyeVector);
}

void main() {
    vec3 pixelColor = vec3(0.0);
    renderImage(pixelColor, gl_FragCoord);
    fs_color = vec4(pixelColor, 1.0);
}
