#ifndef OPENGLRENDERCONTEXT_HPP
#define OPENGLRENDERCONTEXT_HPP

#pragma once

#include <sdfraymarching/render/ShaderProgram.hpp>

class Camera;

class OpenGLRenderContext {
public:
    OpenGLRenderContext() = default;
    ~OpenGLRenderContext() = default;

    ShaderProgram* getShaderProgram() const;
    void setShaderProgram(ShaderProgram* shaderProgram);

    Camera* getCamera() const;
    void setCamera(Camera* camera);

    void setResolution(const glm::ivec2& resolution);
    const glm::ivec2& getResolution() const;

private:
    ShaderProgram* shaderProgram;
    Camera* camera;
    glm::ivec2 resolution;
};

#endif