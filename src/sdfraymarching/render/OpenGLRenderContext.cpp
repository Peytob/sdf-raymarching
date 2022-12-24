#include "OpenGLRenderContext.hpp"

ShaderProgram* OpenGLRenderContext::getShaderProgram() const {
    return shaderProgram;
}

void OpenGLRenderContext::setShaderProgram(ShaderProgram* shaderProgram) {
    this->shaderProgram = shaderProgram;
}
