#include <string>

#include <GL/glew.h>

#include <sdfraymarching/utils/Logger.hpp>

#include <sdfraymarching/render/OpenGLRenderCreatingException.hpp>
#include <sdfraymarching/render/OpenGLResourceCreatingException.hpp>
#include <sdfraymarching/render/OpenGLRenderContext.hpp>
#include <sdfraymarching/render/OpenGLRender.hpp>
#include <sdfraymarching/render/ShaderProgram.hpp>
#include <sdfraymarching/render/Camera.hpp>

#include "Application.hpp"

Application::Application() {
    const int windowWidth = 800;
    const int windowHeigth = 600;

    Logger::info("Initializing raymarching demo application.");
    try {
        this->renderer = new OpenGLRender(windowWidth, windowHeigth, "Ray marching");
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

    this->camera = new Camera(
        {0.0, 0.0, 0.0},
        {1.0, 0.0, 0.0},
        glm::radians(90.0),
        windowWidth / windowHeigth
    );
}

Application::~Application() {
    Logger::info("Destroying raymarching demo application.");
    delete this->renderer;
    delete this->worldShaderProgram;
    delete this->camera;
}

int Application::start() {
    OpenGLRenderContext renderContext;
    renderContext.setShaderProgram(worldShaderProgram);
    renderContext.setCamera(camera);

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
