#ifndef SHADERPROGRAM_HPP
#define SHADERPROGRAM_HPP

#pragma once

#include <GL/gl.h>
#include <glm/fwd.hpp>
#include <string>

class Shader;

class ShaderProgram
{
public:
    static ShaderProgram* loadShaderProgram(const std::string& vertexShaderFile, const std::string& fragmentShaderFile);

    ShaderProgram(const Shader& vertexShader, const Shader& fragmentShader);
    ~ShaderProgram();

    GLint getId() const;

    void setUniform(const std::string& name, const glm::mat4& data);

private:
    const GLint id;
};

#endif