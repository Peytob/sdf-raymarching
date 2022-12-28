#version 330 core

out vec4 fs_color;

uniform ivec2 u_resolution;
uniform mat4 u_view;
uniform vec3 u_cameraPosition;

const float FOV = 90; // TODO Получать из u_view
const int MAX_STEPS = 255;
const float MIN_DISTANCE = 0.0;
const float MAX_DISTANCE = 100.0;
const float EPSILON = 0.001;

/* - = - SDF - = - */

struct SceneObject {
    int id;
    float distance;
};

SceneObject map(vec3 point) {
    point = mod(point, 4.0) - 4.0 * 0.5;
    float sphereDistance = length(point) - 1.0;
    return SceneObject(
        1,
        sphereDistance);
}

/* - = - Ray Marching - = - */

bool rayMarching(in vec3 rayOrigin, in vec3 rayDirection, out SceneObject sceneObject) {
    float dist = 0.0;

    for (int i = 0; i < MAX_STEPS; ++i) {
        vec3 currentPoint = rayOrigin + dist * rayDirection;
        SceneObject mappedObject = map(currentPoint);

        if (abs(mappedObject.distance) < EPSILON) {
            break;
        }

        dist += mappedObject.distance;

        if (dist > MAX_DISTANCE) {
            return false;
        }
    }

    sceneObject.distance = dist;
    return true;
}

/*
 * Вычисление направления луча для одного пикселя.
*/
vec3 computeRayDirection(in float fov, in vec2 resolution, in vec2 fragCoord){
    vec2 xy = fragCoord - resolution * 0.5;
    float z = resolution.y / tan(radians(fov) * 0.5);
    return normalize(vec3(xy, -z));
}

void renderImage(inout vec3 pixelColor, in vec4 gl_FragCoord) {
    vec3 rayDirection = computeRayDirection(FOV, vec2(u_resolution.xy), gl_FragCoord.xy);
    vec3 viewDirection = (u_view * vec4(rayDirection, 0.0)).xyz;

    SceneObject nearestObject = SceneObject(0, 0.0);
    bool isObjectHitted = rayMarching(u_cameraPosition, viewDirection, nearestObject);

    if (isObjectHitted) {
        pixelColor += 3.0 / nearestObject.distance;
    }
}

void main() {
    vec3 pixelColor = vec3(0.0);
    renderImage(pixelColor, gl_FragCoord);
    fs_color = vec4(pixelColor, 1.0);
}
