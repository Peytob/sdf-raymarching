#include <algorithm>

#include <glm/gtx/common.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Camera.hpp"

namespace {
    const glm::vec3 WORLD_UP_VECTOR = {0.0f, 1.0f, 0.0f};
}

Camera::Camera(const glm::vec3& position, float yaw, float pitch, const float fov, const float aspect) :
    position(position),
    upVector(WORLD_UP_VECTOR),
    yaw(yaw),
    pitch(pitch),
    fov(fov),
    aspect(aspect) {

    updateVectors();
}

void Camera::setPosition(const glm::vec3& position) {
    this->position = position;
}

void Camera::move(const glm::vec3& difference) {
    this->position += difference;
}

const glm::vec3& Camera::getPosition() const {
    return position;
}

void Camera::setFrontVector(const glm::vec3& frontVector) {
    this->frontVector = glm::normalize(frontVector);
}

const glm::vec3& Camera::getFrontVector() const {
    return this->frontVector;
}

void Camera::setFov(const float fov) {
    this->fov = glm::fmod(fov, 180.0f);
}

float Camera::getFov() const {
    return fov;
}

void Camera::setAspect(float aspect) {
    this->aspect = aspect;
}

float Camera::getAspect() const {
    return this->aspect;
}

const glm::vec3& Camera::getRigthVector() const {
    return this->rightVector;
}

void Camera::rotate(float xDif, float yDif) {
    yaw += xDif;
    pitch += yDif;

    pitch = std::min(pitch, 89.0f);
    pitch = std::max(pitch, -89.0f);
}

glm::mat4 Camera::computeProjectionMatrix() {
    updateVectors();
    return glm::perspective(fov / 2.0f, aspect, 0.1f, 100.0f);
}

glm::mat4 Camera::computeLookAtMatrix() {
    updateVectors();
    return glm::transpose(glm::lookAtRH(position, position + frontVector, upVector));
}

void Camera::updateVectors() {
    glm::vec3 newFront = {
        cos(glm::radians(yaw)) * cos(glm::radians(pitch)),
        sin(glm::radians(pitch)),
        sin(glm::radians(yaw)) * cos(glm::radians(pitch))
    };

    frontVector = glm::normalize(newFront);

    rightVector = glm::normalize(glm::cross(frontVector, WORLD_UP_VECTOR));
    upVector = glm::normalize(glm::cross(rightVector, frontVector));
}
