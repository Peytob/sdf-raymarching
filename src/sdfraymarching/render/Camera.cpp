#include <algorithm>

#include <glm/gtx/common.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Camera.hpp"

Camera::Camera(const glm::vec3& position, const glm::vec3& frontVector, const float fov, const float aspect) :
    position(position),
    frontVector(frontVector),
    worldUpVector(0.0f, 1.0f, 0.0f),
    yaw(-90.0f),
    pitch(0.0f),
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
    this->fov = glm::fmod(fov, 2.0f);
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

void Camera::rotate(float xDif, float yDif) {
    yaw += xDif;
    pitch += yDif;

    pitch = std::max(pitch, 90.0f);
    pitch = std::min(pitch, -90.0f);
}

glm::mat4 Camera::computeProjectionMatrix() {
    updateVectors();
    return glm::perspective(glm::radians(fov), aspect, 0.1f, 100.0f);
}

glm::mat4 Camera::computeLookAtMatrix() {
    updateVectors();
    return glm::lookAt(position, position + frontVector, worldUpVector);
}

void Camera::updateVectors() {
    glm::vec3 newFront = {
        cos(glm::radians(yaw)) * cos(glm::radians(pitch)),
        sin(glm::radians(pitch)),
        sin(glm::radians(yaw)) * cos(glm::radians(pitch))
    };

    frontVector = glm::normalize(newFront);

    rightVector = glm::normalize(glm::cross(frontVector, worldUpVector));
    worldUpVector = glm::normalize(glm::cross(rightVector, frontVector));
}
