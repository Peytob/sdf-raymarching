#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#pragma once

#include <string>
#include <glm/vec3.hpp>

class Material {
public:
    Material(const std::string& materialId) :
        color(glm::vec3(0.0)),
        materialId(materialId) {};

    ~Material() = default;

    const glm::vec3& getColor() const;
    glm::vec3 setColor(const glm::vec3& color);

    const std::string& getMateialId() const;

private:
    glm::vec3 color;
    const std::string materialId;
};

#endif