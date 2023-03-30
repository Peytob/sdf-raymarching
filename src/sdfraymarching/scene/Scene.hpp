#ifndef SCENE_HPP
#define SCENE_HPP

#pragma once

#include <map>
#include <vector>

#include <sdfraymarching/scene/SceneNode.hpp>
#include <sdfraymarching/scene/Material.hpp>

class Scene {
public:
    Scene();
    Scene(SceneNode* sceneNode);
    ~Scene();

    SceneNode* getRootNode();
    const SceneNode* getRootNode() const;
    const std::vector<Material>& getMaterials() const;
    void addMaterial(const Material& material);

private:
    SceneNode* rootNode;
    std::vector<Material> materials;
};

#endif
