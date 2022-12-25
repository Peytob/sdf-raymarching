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
        {0.0, 0.0, -2.0},
        {0.0, 0.0, -1.0},
        90.0,
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
    renderer->updateStaticUniforms(renderContext);

    while (!renderer->isClosed()) {
        processControl();
        renderer->pullEvents();
        renderer->clear();
        renderer->updateDynamicUniforms(renderContext);
        renderer->draw(renderContext);
        renderer->display();
    }

    return 0;
}


void Application::processControl() {
    const float moveSpeed = 0.25f;
    const float mouseSensivity = 0.75f;

    glm::vec2 cursorDelta = renderer->getCursorDelta();

    if (glm::length(cursorDelta) > FLT_EPSILON) {
        camera->rotate(cursorDelta.x * mouseSensivity, -cursorDelta.y * mouseSensivity);
    }

    glm::vec3 moveVector(0.0f);
    bool isMoved = false;

    if (renderer->getKeyStatus(GLFW_KEY_W) == GLFW_PRESS) {
        moveVector += camera->getFrontVector() * moveSpeed;
        isMoved = true;
    }

    if (renderer->getKeyStatus(GLFW_KEY_S) == GLFW_PRESS) {
        moveVector -= camera->getFrontVector() * moveSpeed;
        isMoved = true;
    }

    if (renderer->getKeyStatus(GLFW_KEY_D) == GLFW_PRESS) {
        moveVector += camera->getRigthVector() * moveSpeed;
        isMoved = true;
    }

    if (renderer->getKeyStatus(GLFW_KEY_A) == GLFW_PRESS) {
        moveVector -= camera->getRigthVector() * moveSpeed;
        isMoved = true;
    }

    if (isMoved) {
        camera->move(moveVector);
    }

    renderer->resetCursor();
}
