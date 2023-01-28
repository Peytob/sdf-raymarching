#ifndef SCENE_HPP
#define SCENE_HPP

#include <sdfraymarching/render/ShaderStorageBuffer.hpp>
#include <sdfraymarching/scene/SceneNode.hpp>

#pragma once

class Scene {
public:
    Scene();
    ~Scene();

    ShaderStorageBuffer* getStorageBuffer();
    SceneNode* getRootNode();

private:
    SceneNode* rootNode;
};

#endif
