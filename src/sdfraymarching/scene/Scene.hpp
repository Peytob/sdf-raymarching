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
    const std::vector<Material>& getMaterials();
    void addMaterial(const Material& material);

private:
    SceneNode* rootNode;
    std::vector<Material> materials;
};

#endif
