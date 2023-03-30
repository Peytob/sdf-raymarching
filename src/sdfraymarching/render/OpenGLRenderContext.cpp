#include "OpenGLRenderContext.hpp"

void OpenGLRenderContext::setCamera(Camera* camera) {
    this->camera = camera;
}

Camera* OpenGLRenderContext::getCamera() const {
    return this->camera;
}

void OpenGLRenderContext::setResolution(const glm::ivec2& resolution) {
    this->resolution = resolution;
}

const glm::ivec2& OpenGLRenderContext::getResolution() const {
    return this->resolution;
}
