#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#pragma once

#include <string>
#include <GL/gl.h>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

class Material {
public:

    struct Plain {
        glm::vec4 color;
        glm::vec4 specColor;
        GLfloat shininess;
    };

    Material(const std::string& materialId) :
        color(glm::vec3(0.0)),
        specColor(glm::vec3(0.0)),
        shininess(0.0),
        materialId(materialId) {};

    ~Material() = default;

    const glm::vec3& getColor() const;
    void setColor(const glm::vec3& color);

    const glm::vec3& getSpecColor() const;
    void setSpecColor(const glm::vec3& specColor);

    const float& getShininess() const;
    void setShininess(const float& shininess);

    const std::string& getMateialId() const;

private:
    glm::vec3 color;
    glm::vec3 specColor;
    float shininess;

    const std::string materialId;
};

#endif