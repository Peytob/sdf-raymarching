#include <sdfraymarching/utils/Logger.hpp>

#include "Application.hpp"

Application::Application() {
    Logger::info("Initializing raymarching demo application.");
    this->renderer = new OpenGLRender();
}

Application::~Application() {
    Logger::info("Destroying raymarching demo application.");
    delete this->renderer;
}
