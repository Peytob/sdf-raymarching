#include "Material.hpp"

const glm::vec3& Material::getColor() const {
    return this->color;
}

void Material::setColor(const glm::vec3& color) {
    this->color = color;
}

const std::string& Material::getMateialId() const {
    return this->materialId;
}

const glm::vec3& Material::getSpecColor() const {
    return this->specColor;
}

void Material::setSpecColor(const glm::vec3& specColor) {
    this->specColor = specColor;
}

const float& Material::getShininess() const {
    return this->shininess;
}

void Material::setShininess(const float& shininess) {
    this->shininess = shininess;
}
