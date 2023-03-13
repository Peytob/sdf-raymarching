#ifndef PLAINSCENE_HPP
#define PLAINSCENE_HPP

#pragma once

#include <vector>

#include <sdfraymarching/scene/Material.hpp>
#include <sdfraymarching/scene/SceneNode.hpp>

struct PlainScene {
    std::vector<SceneNode::Plain> nodes;
    std::vector<Material::Plain> materials;
};


#endif
