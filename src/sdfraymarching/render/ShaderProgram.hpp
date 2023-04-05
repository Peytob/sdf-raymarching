#ifndef SHADERPROGRAM_HPP
#define SHADERPROGRAM_HPP

#pragma once

#include <GL/gl.h>
#include <glm/glm.hpp>

#include <map>
#include <string>

class Shader;

class ShaderProgram
{
public:
    static ShaderProgram* loadShaderProgram(const std::string& vertexShaderFile, const std::string& fragmentShaderFile, const std::map<std::string, std::string>& generatedIncludes = std::map<std::string, std::string>());

    ShaderProgram(const Shader& vertexShader, const Shader& fragmentShader);
    ~ShaderProgram();

    GLint getId() const;

    void setUniform(const std::string& name, const glm::mat4& data);
    void setUniform(const std::string& name, const glm::ivec2& data);
    void setUniform(const std::string& name, const glm::vec3& data);

private:
    const GLint id;

    GLuint getUniformLocation(const std::string& name);
};

#endif