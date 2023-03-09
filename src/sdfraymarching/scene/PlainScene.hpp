#ifndef PLAINSCENE_HPP
#define PLAINSCENE_HPP

#pragma once

#include <vector>
#include <sdfraymarching/scene/SceneNode.hpp>

struct PlainScene {
    std::vector<SceneNode::Plain> nodes;
};


#endif
