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
    std::map<std::string, size_t> materialIdsIndexes;

    void reset();
    GLint processNode(SceneNode* node, glm::vec3 localPosition);
    void processMaterials(const std::vector<Material>& materials);
};

#endif
