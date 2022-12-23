#include <GL/glew.h>

#include <sdfraymarching/render/OpenGLResourceCreatingException.hpp>
#include <sdfraymarching/render/Shader.hpp>

#include "ShaderProgram.hpp"

ShaderProgram::ShaderProgram(const Shader& vertexShader, const Shader& fragmentShader) :
    id(glCreateProgram()) {

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
        std::string message = "Error while linking shader program with id " + std::to_string(id);
        throw OpenGLResourceCreatingException(message);
    }
}

ShaderProgram::~ShaderProgram() {
    glDeleteProgram(id);

}

GLint ShaderProgram::getId() const {
    return id;
}

