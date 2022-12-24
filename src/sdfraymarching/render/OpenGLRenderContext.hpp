#ifndef OPENGLRENDERCONTEXT_HPP
#define OPENGLRENDERCONTEXT_HPP

#pragma once

#include <sdfraymarching/render/ShaderProgram.hpp>

class OpenGLRenderContext {
public:
    OpenGLRenderContext() = default;
    ~OpenGLRenderContext() = default;

    ShaderProgram* getShaderProgram() const;
    void setShaderProgram(ShaderProgram* shaderProgram);

private:
    ShaderProgram* shaderProgram;
};

#endif