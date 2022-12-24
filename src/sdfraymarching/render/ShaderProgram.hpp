#ifndef SHADERPROGRAM_HPP
#define SHADERPROGRAM_HPP

#pragma once

#include <GL/gl.h>

class Shader;

class ShaderProgram
{
public:
    static ShaderProgram* loadShaderProgram(const std::string& vertexShaderFile, const std::string& fragmentShaderFile);

    ShaderProgram(const Shader& vertexShader, const Shader& fragmentShader);
    ~ShaderProgram();

    GLint getId() const;

private:
    const GLint id;
};

#endif