#include <sdfraymarching/scene/Scene.hpp>
#include <sdfraymarching/scene/SceneLoadException.hpp>

#include <nlohmann/json.hpp>

#include "JsonSceneLoader.hpp"

namespace {

const std::string SCENE_ROOT_OBJECT = "scene";

const std::string OPERATION_FIELD = "operation";
const std::string LEAF_OPERATION_OBJECT = "leaf";
const std::string MERGE_OPERATION_OBJECT = "merge";

const std::string LEFT_OPERAND_OBJECT = "left";
const std::string RIGHT_OPERAND_OBJECT = "right";

const std::string FIGURE_OBJECT = "figure";

glm::vec3 parseVec3(const nlohmann::json& vecNode) {
    return glm::vec3(
        vecNode["x"].get<float>(),
        vecNode["y"].get<float>(),
        vecNode["z"].get<float>()
    );
}

Figure parseFigure(const nlohmann::json& figureNode) {
    const std::string figureType = figureNode["type"].get<std::string>();
    Figure figure;

    if (figureType == "sphere") {
        figure.type = SPHERE;
        figure.details.asSphere.radius = figureNode["radius"].get<float>();
    } else if (figureType == "box") {
        figure.type = BOX;
        figure.details.asBox.sizes = parseVec3(figureNode["sizes"]);
    } else if (figureType == "torus") {
        figure.type = TORUS;
        figure.details.asTorus.largeRadius = figureNode["largeRadius"].get<float>();
        figure.details.asTorus.smallRadius = figureNode["smallRadius"].get<float>();
    } else if (figureType == "plane") {
        figure.type = PLANE;
        figure.details.asPlane.normal = parseVec3(figureNode["normal"]);
        figure.details.asPlane.distanceFromOrigin = figureNode["distanceFromOrigin"].get<float>();
    } else if (figureType == "cylinder") {
        figure.type = CYLINDER;
        figure.details.asCylinder.radius = figureNode["radius"].get<float>();
        figure.details.asCylinder.height = figureNode["height"].get<float>();
    } else {
        throw SceneLoadException("Figure with type " + figureType + " not exists");
    }

    return figure;
}

SceneNode* parseSceneNode(const nlohmann::json& jsonNode) {
    const std::string operation = jsonNode[OPERATION_FIELD].get<std::string>();

    if (operation == LEAF_OPERATION_OBJECT) {
        Figure figure = parseFigure(jsonNode[FIGURE_OBJECT]);
        return SceneNode::figure(figure);
    }

    SceneNode* leftNode = parseSceneNode(jsonNode[LEFT_OPERAND_OBJECT]);
    SceneNode* rightNode = parseSceneNode(jsonNode[RIGHT_OPERAND_OBJECT]);

    throw SceneLoadException("Operation with type " + operation + " not exists");
    return nullptr; // TODO Just mock.
}

} // namespace

Scene* JsonSceneLoader::load(const std::string& jsonString) {
    const nlohmann::json sceneJson = nlohmann::json::parse(jsonString);
    SceneNode* root = parseSceneNode(sceneJson["scene"]);
    return new Scene(root);
}
