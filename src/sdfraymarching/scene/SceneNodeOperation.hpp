#ifndef SCENENODEOPERATION_HPP
#define SCENENODEOPERATION_HPP

#include <GL/gl.h>

#pragma once

enum SceneNodeOperation : GLint {
    LEAF = 0,
    SUBSTRACTION = 1,
    MERGE = 2,
    INTERSECTION = 3,
};

#endif