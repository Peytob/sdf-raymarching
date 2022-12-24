#include <GL/glew.h>
#include <sdfraymarching/render/OpenGLResourceCreatingException.hpp>
#include <sdfraymarching/utils/Logger.hpp>

#include "Shader.hpp"

Shader::Shader(const std::string& code, GLenum type) :
    type(type),
    id(glCreateShader(type)) {

    Logger::info("Created shader with id " + std::to_string(id));

    const char *c_str = code.c_str();
    glShaderSource(id, 1, &c_str, nullptr);
    glCompileShader(id);

    GLint isCompilingSuccess;
    glGetShaderiv(id, GL_COMPILE_STATUS, &isCompilingSuccess);
    if (!isCompilingSuccess) {
        const size_t infoLogSize = 512;
        char infoLog[infoLogSize];
        glGetShaderInfoLog(id, infoLogSize, nullptr, infoLog);
        std::string message = "Error while compiling shader with id " + std::to_string(id) + ": " + std::string(infoLog);
        throw OpenGLResourceCreatingException(message);
    }
}

Shader::~Shader() {
    glDeleteShader(id);
    Logger::info("Deleted shader with id " + std::to_string(id));
}

GLenum Shader::getType() const {
    return type;
}

GLint Shader::getId() const {
    return id;
}
