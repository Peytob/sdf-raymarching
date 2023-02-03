#include <sdfraymarching/scene/Scene.hpp>

#include "JsonSceneLoader.hpp"

Scene* JsonSceneLoader::load(const std::string& jsonString) {
    SceneNode* root = SceneNode::figure(Figure(SPHERE, FigureDetails()));

    return new Scene(root);
}
