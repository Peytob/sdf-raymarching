#ifndef SCENETOPLAINCONVERTER_HPP
#define SCENETOPLAINCONVERTER_HPP

#pragma once

#include <sdfraymarching/scene/Scene.hpp>
#include <sdfraymarching/scene/PlainScene.hpp>

class SceneToPlainConverter {
public:
    SceneToPlainConverter() = default;
    ~SceneToPlainConverter() = default;

    PlainScene toPlainData(const Scene* scene);
    const std::map<std::string, size_t>& getMaterialsIndexesMap() const;

private:
    PlainScene plainScene;
    std::map<std::string, size_t> materialIdsIndexes;

    void reset();
    GLint processNode(const SceneNode* node, const glm::vec3& localPosition);
    void processMaterials(const std::vector<Material>& materials);
};

#endif
