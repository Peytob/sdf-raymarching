#include "Scene.hpp"

Scene::Scene() : Scene(SceneNode::merge()) { }

Scene::Scene(SceneNode* sceneNode) :
    rootNode(sceneNode) {}

Scene::~Scene() {
    delete this->rootNode;
}

SceneNode* Scene::getRootNode() {
    return this->rootNode;
}

const SceneNode* Scene::getRootNode() const {
    return this->rootNode;
}

const std::vector<Material>& Scene::getMaterials() const {
    return this->materials;
}

void Scene::addMaterial(const Material& material) {
    this->materials.push_back(material);
}
