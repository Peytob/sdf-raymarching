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
        // TODO Hide GLFW features
        this->renderer->setKeyCallback([](GLFWwindow* window, int key, int scancode, int action, int mode) {
            OpenGLRender* render = static_cast<OpenGLRender*>(glfwGetWindowUserPointer(window));
            if (key == GLFW_KEY_ESCAPE) {
                render->close();
            }
        });
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

    renderer->resetCursor();

    while (!renderer->isClosed()) {
        processControl();
        renderer->pullEvents();
        renderer->clear();
        renderer->draw(renderContext);
        renderer->display();
    }

    return 0;
}


void Application::processControl() {
    glm::vec2 cursorPosition = renderer->getCursorDelta();

    if (glm::length(cursorPosition) > FLT_EPSILON) {
        Logger::info("Cursor: " + std::to_string(cursorPosition.x) + "; " + std::to_string(cursorPosition.y));
    }

    if (renderer->getKeyStatus(GLFW_KEY_A) == GLFW_PRESS) {
        Logger::info("a");
    }

    if (renderer->getKeyStatus(GLFW_KEY_D) == GLFW_PRESS) {
        Logger::info("d");
    }

    if (renderer->getKeyStatus(GLFW_KEY_W) == GLFW_PRESS) {
        Logger::info("w");
    }

    if (renderer->getKeyStatus(GLFW_KEY_S) == GLFW_PRESS) {
        Logger::info("s");
    }

    renderer->resetCursor();
}
