#include <GL/glew.h>
#include <sdfraymarching/render/OpenGLResourceCreatingException.hpp>

#include "Shader.hpp"

Shader::Shader(const std::string& code, GLenum type) :
    type(type),
    id(glCreateShader(type)) {

    const char *c_str = code.c_str();
    glShaderSource(id, 1, &c_str, nullptr);
    glCompileShader(id);

    GLint isCompilingSuccess;
    glGetShaderiv(id, GL_COMPILE_STATUS, &isCompilingSuccess);
    if (!isCompilingSuccess) {
        const size_t infoLogSize = 512;
        char infoLog[infoLogSize];
        glGetShaderInfoLog(id, infoLogSize, nullptr, infoLog);
        std::string message = "Error while compiling OGL shader: " + std::string(infoLog);
        throw OpenGLResourceCreatingException(message);
    }
}

Shader::~Shader() {
    glDeleteShader(id);
}

GLenum Shader::getType() const {
    return type;
}

GLint Shader::getId() const {
    return id;
}
