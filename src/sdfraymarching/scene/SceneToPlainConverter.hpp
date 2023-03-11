#ifndef SCENETOPLAINCONVERTER_HPP
#define SCENETOPLAINCONVERTER_HPP

#pragma once

#include <sdfraymarching/scene/Scene.hpp>
#include <sdfraymarching/scene/PlainScene.hpp>

class SceneToPlainConverter {
public:
    SceneToPlainConverter() = default;
    ~SceneToPlainConverter() = default;

    PlainScene toPlainData(Scene* scene);

private:
    PlainScene plainScene;

    void reset();
    int processNode(SceneNode* node);
};

#endif
