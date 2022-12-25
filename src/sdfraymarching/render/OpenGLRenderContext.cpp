#include "OpenGLRenderContext.hpp"

ShaderProgram* OpenGLRenderContext::getShaderProgram() const {
    return shaderProgram;
}

void OpenGLRenderContext::setShaderProgram(ShaderProgram* shaderProgram) {
    this->shaderProgram = shaderProgram;
}

void OpenGLRenderContext::setCamera(Camera* camera) {
    this->camera = camera;
}

Camera* OpenGLRenderContext::getCamera() const {
    return this->camera;
}
