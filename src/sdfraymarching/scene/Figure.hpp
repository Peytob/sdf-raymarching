#ifndef FIGURE_HPP
#define FIGURE_HPP

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#pragma once

enum FigureType : GLint {
    SPHERE = 1,
    BOX = 2,
    TORUS = 3,
    PLANE = 4,
    CYLINDER = 5,
};

struct Sphere {
    float radius;
};

struct Box {
    glm::vec3 sizes;
};

struct Torus {
    float smallRadius;
    float largeRadius;
};

struct Plane {
    glm::vec3 normal;
    float distanceFromOrigin;
};

struct Cylinder {
    float radius;
    float height;
};

union FigureDetails {
    Sphere asSphere;
    Box asBox;
    Torus asTorus;
    Plane asPlane;
    Cylinder asCylinder;
    float asRawData[12];
};

struct Figure {
    FigureType type;
    FigureDetails details;

    Figure() = default;

    Figure(FigureType type, FigureDetails details) :
        type(type),
        details(details) {};
};

#endif
