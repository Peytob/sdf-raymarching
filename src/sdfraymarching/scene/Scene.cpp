#include "Scene.hpp"

Scene::Scene() : Scene(SceneNode::merge()) { }

Scene::Scene(SceneNode* sceneNode) {
    this->rootNode = sceneNode;
}

Scene::~Scene() {
    delete this->rootNode;
}

SceneNode* Scene::getRootNode() {
    return this->rootNode;
}
