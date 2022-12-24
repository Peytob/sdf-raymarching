#include <string>

#include <GL/glew.h>

#include <sdfraymarching/utils/Logger.hpp>

#include <sdfraymarching/render/OpenGLRenderCreatingException.hpp>
#include <sdfraymarching/render/OpenGLResourceCreatingException.hpp>
#include <sdfraymarching/render/OpenGLRenderContext.hpp>

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

    try {
        this->worldShaderProgram = ShaderProgram::loadShaderProgram("./resources/vertex.glsl", "./resources/fragment.glsl");
    } catch (const OpenGLResourceCreatingException& e) {
        Logger::error("Error while loading world shader program. Description: " + std::string(e.what()));
        std::exit(1);
    }
}

Application::~Application() {
    Logger::info("Destroying raymarching demo application.");
    delete this->renderer;
    delete this->worldShaderProgram;
}

int Application::start() {
    OpenGLRenderContext renderContext;
    renderContext.setShaderProgram(worldShaderProgram);

    while (true) {
        renderer->pullEvents();
        renderer->clear();
        renderer->draw(renderContext);
        renderer->display();
    }
    
    return 0;
}
