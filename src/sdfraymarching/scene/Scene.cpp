#include "Scene.hpp"

Scene::Scene() {
    this->rootNode = SceneNode::merge();
}

Scene::~Scene() {
    delete this->rootNode;
}