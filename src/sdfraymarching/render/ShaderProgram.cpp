#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <sdfraymarching/render/OpenGLResourceCreatingException.hpp>
#include <sdfraymarching/render/ShaderPreprocessor.hpp>
#include <sdfraymarching/render/Shader.hpp>

#include <sdfraymarching/utils/Logger.hpp>
#include <sdfraymarching/utils/FileUtils.hpp>

#include "ShaderProgram.hpp"

ShaderProgram* ShaderProgram::loadShaderProgram(const std::string& vertexShaderFile, const std::string& fragmentShaderFile, const std::map<std::string, std::string>& generatedIncludes) {
    auto vertexFiles = FileUtils::splitFolderAndFilename(vertexShaderFile);
    Shader vertexShader = ShaderPreprocessor::loadShaderWithPreprocessor(
        vertexFiles.first,
        vertexFiles.second,
        GL_VERTEX_SHADER,
        generatedIncludes);

    auto fragmentFiles = FileUtils::splitFolderAndFilename(fragmentShaderFile);
    Shader fragmentShader = ShaderPreprocessor::loadShaderWithPreprocessor(
        fragmentFiles.first,
        fragmentFiles.second,
        GL_FRAGMENT_SHADER,
        generatedIncludes);

    return new ShaderProgram(vertexShader, fragmentShader);
}

ShaderProgram::ShaderProgram(const Shader& vertexShader, const Shader& fragmentShader) :
    id(glCreateProgram()) {

    Logger::info("Created shader program with id " + std::to_string(id));

    if (vertexShader.getType() != GL_VERTEX_SHADER) {
        throw OpenGLResourceCreatingException("Vertex shader is not a vertex shader!");
    }

    if (fragmentShader.getType() != GL_FRAGMENT_SHADER) {
        throw OpenGLResourceCreatingException("Fragment shader is not a fragmet shader!");
    }

    glAttachShader(id, vertexShader.getId());
    glAttachShader(id, fragmentShader.getId());
    glLinkProgram(id);

    GLint isLinkingSuccess;
    glGetProgramiv(id, GL_LINK_STATUS, &isLinkingSuccess);
    if (!isLinkingSuccess) {
        const size_t infoLogSize = 512;
        char infoLog[infoLogSize];
        glGetProgramInfoLog(id, infoLogSize, nullptr, infoLog);
        std::string message = "Error while linking shader program with id " + std::to_string(id) + ": " + std::string(infoLog);
        throw OpenGLResourceCreatingException(message);
    }
}

ShaderProgram::~ShaderProgram() {
    glDeleteProgram(id);
    Logger::info("Deleted shader program with id " + std::to_string(id));
}

GLint ShaderProgram::getId() const {
    return id;
}

GLuint ShaderProgram::getUniformLocation(const std::string& name) {
    GLint location = glGetUniformLocation(id, name.c_str());

    if (location == -1) {
        Logger::error("Uniform " + name + " not found!");
    }

    return location;
}

void ShaderProgram::setUniform(const std::string& name, const glm::mat4& data) {
    GLint location = getUniformLocation(name);
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(data));
}

void ShaderProgram::setUniform(const std::string& name, const glm::ivec2& data) {
    GLint location = getUniformLocation(name);
    glUniform2i(location, data.x, data.y);
}

void ShaderProgram::setUniform(const std::string& name, const glm::vec3& data) {
    GLint location = getUniformLocation(name);
    glUniform3f(location, data.x, data.y, data.z);
}
