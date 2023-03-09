#include <sdfraymarching/scene/Scene.hpp>
#include <sdfraymarching/scene/SceneLoadException.hpp>
#include <sdfraymarching/utils/Logger.hpp>

#include <nlohmann/json.hpp>

#include "JsonSceneLoader.hpp"

namespace {

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

SceneNode* createOperationNode(const std::string& operation) {
    if (operation == "merge") {
        return SceneNode::merge();
    } else if (operation == "substraction") {
        return SceneNode::substraction();
    } else if (operation == "intersection") {
        return SceneNode::intersection();
    }

    throw SceneLoadException("Operation with type " + operation + " not exists");
    return nullptr; // TODO Just mock to suppress warning.
}

SceneNode* parseSceneNode(const nlohmann::json& jsonNode) {
    const std::string operation = jsonNode["operation"].get<std::string>();

    if (operation == "leaf") {
        Figure figure = parseFigure(jsonNode["figure"]);
        return SceneNode::figure(figure);
    }

    SceneNode* leftNode = parseSceneNode(jsonNode["left"]);
    SceneNode* rightNode = parseSceneNode(jsonNode["right"]);
    SceneNode* operationNode = createOperationNode(operation);

    operationNode->setLeftChild(leftNode);
    operationNode->setRightChild(rightNode);

    return operationNode;
}

} // namespace

Scene* JsonSceneLoader::load(const std::string& jsonString) {
    Logger::info("Parsing scene JSON");
    const nlohmann::json sceneJson = nlohmann::json::parse(jsonString);
    SceneNode* root = parseSceneNode(sceneJson["scene"]);
    return new Scene(root);
}
