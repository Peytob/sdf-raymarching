#ifndef FIGURE_HPP
#define FIGURE_HPP

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#pragma once

enum FigureType {
    PLANE,
    SPHERE
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
    glm::vec3 position;
    glm::vec3 normal;
    float distanceFromOrigin;
};

struct Cylinder {
    float radius;
    float height;
};

union FigureDetails {
    Sphere asSpehere;
    Box asBox;
    Torus asTorus;
    Plane asPlane;
    Cylinder asCylinder;
};

struct Figure {
    FigureType type;
    FigureDetails details;
};

#endif
