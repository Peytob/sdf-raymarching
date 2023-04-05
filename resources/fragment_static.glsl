#pragma include "fragment.glsl"

void map(vec3 point, out SceneObject sceneObject) {
    sceneObject.distanceFromOrigin = sphereSdf(point, 1.0);
}
