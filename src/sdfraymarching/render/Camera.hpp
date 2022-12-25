#ifndef CAMERA_HPP
#define CAMERA_HPP

#pragma once

#include <glm/glm.hpp>

class Camera {
public:
    Camera(const glm::vec3& position, const glm::vec3& frontVector, const float fov, const float aspect);
    ~Camera() = default;

    void setPosition(const glm::vec3& position);
    const glm::vec3& getPosition() const;
    void move(const glm::vec3& difference);

    void setFrontVector(const glm::vec3& frontVector);
    const glm::vec3& getFrontVector() const;

    void setAspect(float aspect);
    float getAspect() const;

    void setFov(const float fov);
    float getFov() const;

    void rotate(float xDif, float yDif);

    glm::mat4 computeProjectionMatrix();
    glm::mat4 computeLookAtMatrix();

private:
    glm::vec3 position;
    glm::vec3 frontVector;
    glm::vec3 worldUpVector;
    glm::vec3 rightVector;

    float yaw;
    float pitch;

    float fov;
    float aspect;

    void updateVectors();
};

#endif