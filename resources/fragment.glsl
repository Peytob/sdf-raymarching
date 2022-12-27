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

struct SceneObject {
    float rayDistance;
};

/*
 * Вычисление направления луча для одного пикселя.
*/
vec3 computeRayDirection(in float fov, in vec2 resolution, in vec2 fragCoord){
    vec2 xy = fragCoord - resolution / 2.0;
    float z = resolution.y / tan(radians(fov) * 0.5);
    return normalize(vec3(xy, -z));
}

vec2 map(vec3 point) {
    point = mod(point, 4.0) - 4.0 * 0.5;
    float sphereDistance = length(point) - 1.0;
    return vec2(sphereDistance, 1.0); // distance - id
}

float rayMarching(in vec3 rayOrigin, in vec3 rayDirection) {
    vec2 object = vec2(0.0);
    vec2 hit = vec2(0.0);

    for (int i = 0; i < MAX_STEPS; i++) {
        vec3 currentPoint = rayOrigin + object.x * rayDirection;
        hit = map(currentPoint);
        object.x += hit.x;
        object.y = hit.y;
        if (abs(hit.x) < EPSILON || object.x > MAX_DISTANCE) {
            break;
        }
    }

    return object.x;
}

void renderImage(inout vec3 pixelColor, in vec4 gl_FragCoord) {
    vec3 rayDirection = computeRayDirection(FOV, vec2(u_resolution.xy), gl_FragCoord.xy);
    vec3 viewDirection = (u_view * vec4(rayDirection, 0.0)).xyz;

    float nearestObjectDistance = rayMarching(u_cameraPosition, viewDirection);

    if (nearestObjectDistance < MAX_DISTANCE) {
        pixelColor += 3.0 / nearestObjectDistance;
    }
}

void main() {
    vec3 pixelColor = vec3(0.0);
    renderImage(pixelColor, gl_FragCoord);
    fs_color = vec4(pixelColor, 1.0);
}
