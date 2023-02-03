#ifndef SCENE_HPP
#define SCENE_HPP

#include <sdfraymarching/scene/SceneNode.hpp>

#pragma once

class Scene {
public:
    Scene();
    Scene(SceneNode* sceneNode);
    ~Scene();

    SceneNode* getRootNode();

private:
    SceneNode* rootNode;
};

#endif
