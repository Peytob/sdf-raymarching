#include <string>

#include <sdfraymarching/utils/Logger.hpp>
#include <sdfraymarching/render/OpenGLRenderCreatingException.hpp>

#include "Application.hpp"

Application::Application() {
    Logger::info("Initializing raymarching demo application.");
    try {
        this->renderer = new OpenGLRender(800, 600, "Ray marching");
    }
    catch(const OpenGLRenderCreatingException& e) {
        Logger::error("Error while creating OpenGLRenderer. Description: " + std::string(e.what()));
        std::exit(1);
    }
}

Application::~Application() {
    Logger::info("Destroying raymarching demo application.");
    delete this->renderer;
}
