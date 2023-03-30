#ifndef OPENGLRENDERCONTEXT_HPP
#define OPENGLRENDERCONTEXT_HPP

#pragma once

#include <glm/vec2.hpp>

class Camera;

class OpenGLRenderContext {
public:
    OpenGLRenderContext() = default;
    ~OpenGLRenderContext() = default;

    Camera* getCamera() const;
    void setCamera(Camera* camera);

    void setResolution(const glm::ivec2& resolution);
    const glm::ivec2& getResolution() const;

private:
    Camera* camera;
    glm::ivec2 resolution;
};

#endif