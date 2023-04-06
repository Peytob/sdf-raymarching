#include <sdfraymarching/scene/Scene.hpp>

#include "SceneSdfMethodGenerator.hpp"

namespace {

std::string generateVec3String(const glm::vec3& vec3) {
    return "vec3(" + std::to_string(vec3.x) + ", " + std::to_string(vec3.y) + ", " + std::to_string(vec3.z) + ")";
}

std::string generateLeafSdfCall(const Figure& figure, const std::string& point) {
    if (FigureType::SPHERE == figure.type) {
        const Sphere& sphere = figure.details.asSphere;
        return "sphereSdf(" + point + ", " + std::to_string(sphere.radius) + ")";
    }

    if (FigureType::BOX == figure.type) {
        const Box& box = figure.details.asBox;
        return "boxSdf(" + point + ", " + generateVec3String(box.sizes) + ")";
    }

    if (FigureType::TORUS == figure.type) {
        const Torus& torus = figure.details.asTorus;
        return "torusSdf(" + point + ", " + std::to_string(torus.smallRadius) + ", " + std::to_string(torus.smallRadius) + ")";
    }

    if (FigureType::PLANE == figure.type) {
        const Plane& plane = figure.details.asPlane;
        return "planeSdf(" + point + ", " + generateVec3String(plane.normal) + ", " + std::to_string(plane.distanceFromOrigin) + ")";
    }

    if (FigureType::CYLINDER == figure.type) {
        const Cylinder& cylinder = figure.details.asCylinder;
        return "cylinderSdf(" + point + ", " + std::to_string(cylinder.height) + ", " + std::to_string(cylinder.radius) + ")";
    }

    return "UNDEFINED_LEAF_FIGURE";
}

std::string generateLeafSdf(const SceneNode* node, const std::map<std::string, size_t>& materialIndexes) {
    const Figure& figure = node->getFigure();

    const glm::vec3 position = node->getLocalPosition();
    const std::string point = "point - " + generateVec3String(position);
    const std::string function = generateLeafSdfCall(figure, point);

    const size_t materialId = materialIndexes.at(node->getMaterialId());

    return "SceneObject(" + std::to_string(materialId) + ", " + function + ")";
}

std::string generateNodeSdf(const SceneNode* node, const std::map<std::string, size_t>& materialIndexes) {
    if (SceneNodeOperation::LEAF == node->getOperation()) {
        return generateLeafSdf(node, materialIndexes);
    }

    if (SceneNodeOperation::INTERSECTION == node->getOperation()) {
        return "intersectionOp(" + generateNodeSdf(node->getLeftChild(), materialIndexes) + "," + generateNodeSdf(node->getRightChild(), materialIndexes) + ")";
    }

    if (SceneNodeOperation::MERGE == node->getOperation()) {
        return "mergeOp(" + generateNodeSdf(node->getLeftChild(), materialIndexes) + "," + generateNodeSdf(node->getRightChild(), materialIndexes) + ")";
    }

    if (SceneNodeOperation::SUBTRACTION == node->getOperation()) {
        return "subtractionOp(" + generateNodeSdf(node->getLeftChild(), materialIndexes) + "," + generateNodeSdf(node->getRightChild(), materialIndexes) + ")";
    }

    return "UNDEFINED_SDF_OPERATION";
}

}

std::string SceneSdfMethodGenerator::generateSceneSdfMethod(const Scene* scene, const std::map<std::string, size_t>& materialIndexes) {
    return "void map(vec3 point, out SceneObject sceneObject) {\n"
           "\tsceneObject = " + generateNodeSdf(scene->getRootNode(), materialIndexes) + ";\n"
           "}";
}
