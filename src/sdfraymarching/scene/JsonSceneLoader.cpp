#include <sdfraymarching/scene/Scene.hpp>
#include <sdfraymarching/scene/SceneLoadException.hpp>
#include <sdfraymarching/utils/Logger.hpp>

#include <nlohmann/json.hpp>

#include "JsonSceneLoader.hpp"

namespace {

glm::vec3 parseVec3(const nlohmann::json& vecNode) {
    return {
        vecNode.at(0).get<float>(),
        vecNode.at(1).get<float>(),
        vecNode.at(2).get<float>()
    };
}

Figure parseFigure(const nlohmann::json& figureNode) {
    const std::string figureType = figureNode.at("type").get<std::string>();
    Figure figure;

    if (figureType == "sphere") {
        figure.type = SPHERE;
        figure.details.asSphere.radius = figureNode.at("radius").get<float>();
    } else if (figureType == "box") {
        figure.type = BOX;
        figure.details.asBox.sizes = parseVec3(figureNode.at("sizes"));
    } else if (figureType == "torus") {
        figure.type = TORUS;
        figure.details.asTorus.largeRadius = figureNode.at("largeRadius").get<float>();
        figure.details.asTorus.smallRadius = figureNode.at("smallRadius").get<float>();
    } else if (figureType == "plane") {
        figure.type = PLANE;
        figure.details.asPlane.normal = parseVec3(figureNode.at("normal"));
        figure.details.asPlane.distanceFromOrigin = figureNode.at("distanceFromOrigin").get<float>();
    } else if (figureType == "cylinder") {
        figure.type = CYLINDER;
        figure.details.asCylinder.radius = figureNode.at("radius").get<float>();
        figure.details.asCylinder.height = figureNode.at("height").get<float>();
    } else {
        throw SceneLoadException("Figure with type " + figureType + " not exists");
    }

    return figure;
}

SceneNode* createOperationNode(const std::string& operation) {
    if (operation == "merge") {
        return SceneNode::merge();
    } else if (operation == "subtraction") {
        return SceneNode::subtraction();
    } else if (operation == "intersection") {
        return SceneNode::intersection();
    }

    throw SceneLoadException("Operation with type " + operation + " not exists");
    return nullptr; // TODO Just mock to suppress warning.
}

SceneNode* parseSceneNode(const nlohmann::json& jsonNode) {
    const std::string operation = jsonNode.at("operation").get<std::string>();

    glm::vec3 localPosition = jsonNode.contains("localPosition") ?
            parseVec3(jsonNode.at("localPosition")) :
            glm::vec3(0.0f);

    if (operation == "leaf") {
        Figure figure = parseFigure(jsonNode.at("figure"));
        std::string materialId = jsonNode.at("materialId");
        SceneNode* node = SceneNode::figure(figure, localPosition);
        node->setMaterialId(materialId);
        return node;
    }

    SceneNode* leftNode = parseSceneNode(jsonNode.at("left"));
    SceneNode* rightNode = parseSceneNode(jsonNode.at("right"));
    SceneNode* operationNode = createOperationNode(operation);

    operationNode->setLeftChild(leftNode);
    operationNode->setRightChild(rightNode);
    operationNode->setLocalPosition(localPosition);

    return operationNode;
}

Material parseMaterial(const nlohmann::json& materialNode) {
    std::string id = materialNode.at("id").get<std::string>();
    glm::vec3 color = parseVec3(materialNode.at("color"));
    glm::vec3 specColor = parseVec3(materialNode.at("specColor"));
    float shininess = materialNode.at("shininess").get<float>();

    Material material(id);
    material.setColor(color);
    material.setSpecColor(specColor);
    material.setShininess(shininess);

    return material;
}

} // namespace

Scene* JsonSceneLoader::load(const std::string& jsonString) {
    Logger::info("Parsing scene JSON");
    const nlohmann::json sceneJson = nlohmann::json::parse(jsonString);

    SceneNode* root = parseSceneNode(sceneJson.at("scene"));
    Scene* scene = new Scene(root);

    for (const auto& materialNode : sceneJson.at("material")) {
        scene->addMaterial(parseMaterial(materialNode));
    }

    return scene;
}
