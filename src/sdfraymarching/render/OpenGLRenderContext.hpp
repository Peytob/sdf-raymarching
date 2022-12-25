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

    void setCamera(Camera* camera);
    Camera* getCamera() const;

private:
    ShaderProgram* shaderProgram;
    Camera* camera;
};

#endif