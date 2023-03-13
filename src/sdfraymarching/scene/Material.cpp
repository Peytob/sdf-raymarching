#include "Material.hpp"

const glm::vec3& Material::getColor() const {
    return this->color;
}

glm::vec3 Material::setColor(const glm::vec3& color) {
    this->color = color;
}

const std::string& Material::getMateialId() const {
    return this->materialId;
}
