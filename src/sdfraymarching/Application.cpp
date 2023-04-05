#include <string>

#include <GL/glew.h>

#include <sdfraymarching/utils/Logger.hpp>

#include <sdfraymarching/render/OpenGLRenderCreatingException.hpp>
#include <sdfraymarching/render/OpenGLResourceCreatingException.hpp>
#include <sdfraymarching/render/OpenGLRenderContext.hpp>
#include <sdfraymarching/render/OpenGLWrapper.hpp>
#include <sdfraymarching/render/ShaderProgram.hpp>
#include <sdfraymarching/render/Camera.hpp>
#include <sdfraymarching/utils/FileUtils.hpp>
#include <sdfraymarching/scene/SceneLoadException.hpp>
#include <sdfraymarching/render/drawer/StaticDrawer.hpp>

#include <sdfraymarching/scene/Scene.hpp>
#include <sdfraymarching/scene/JsonSceneLoader.hpp>

#include "Application.hpp"

Application::Application() {
    const int windowWidth = 800;
    const int windowHeigth = 600;
    const std::string sceneFile = "./resources/example_scene.json";

    Logger::info("Initializing raymarching demo application.");

    try {
        this->openGlWrapper = new OpenGLWrapper(windowWidth, windowHeigth, "Ray marching");
        // TODO Hide GLFW features
        this->openGlWrapper->setKeyCallback([](GLFWwindow* window, int key, int scancode, int action, int mode) {
            OpenGLWrapper* render = static_cast<OpenGLWrapper*>(glfwGetWindowUserPointer(window));
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
        JsonSceneLoader jsonSceneLoader = JsonSceneLoader();
        Logger::info("Loading scene JSON from file " + sceneFile);
        this->scene = jsonSceneLoader.load(FileUtils::readFile(sceneFile));
    } catch (const SceneLoadException& e) {
        Logger::error("Error while loading or parsing scene. Description: " + std::string(e.what()));
        std::exit(1);
    }

    try {
        this->drawer = new StaticDrawer(this->openGlWrapper);
        drawer->onSceneLoaded(scene);
    } catch (OpenGLResourceCreatingException exception) {
        Logger::error("Error while creating drawer or loading drawer resources: " + std::string(exception.what()));
    }

    this->camera = new Camera(
        {0.0, 10.0, -5.0},
        -45.0,
        -45.0,
        90.0,
        windowWidth / windowHeigth
    );
}

Application::~Application() {
    Logger::info("Destroying raymarching demo application.");
    delete this->drawer;
    delete this->openGlWrapper;
    delete this->camera;
    delete this->scene;
}

int Application::start() {
    OpenGLRenderContext renderContext;
    renderContext.setCamera(camera);
    renderContext.setResolution(openGlWrapper->getResolution());

    openGlWrapper->resetCursor();

    while (!openGlWrapper->isClosed()) {
        processControl();
        openGlWrapper->pullEvents();
        openGlWrapper->clear();
        drawer->drawScene(scene, renderContext);
        openGlWrapper->display();
    }

    return 0;
}


void Application::processControl() {
    const float moveSpeed = 0.25f;
    const float mouseSensivity = 0.5f;

    glm::vec2 cursorDelta = openGlWrapper->getCursorDelta();

    if (glm::length(cursorDelta) > FLT_EPSILON) {
        camera->rotate(cursorDelta.x * mouseSensivity, -cursorDelta.y * mouseSensivity);
    }

    glm::vec3 moveVector(0.0f);

    if (openGlWrapper->getKeyStatus(GLFW_KEY_W) == GLFW_PRESS) {
        moveVector += camera->getFrontVector() * moveSpeed;
    }

    if (openGlWrapper->getKeyStatus(GLFW_KEY_S) == GLFW_PRESS) {
        moveVector -= camera->getFrontVector() * moveSpeed;
    }

    if (openGlWrapper->getKeyStatus(GLFW_KEY_D) == GLFW_PRESS) {
        moveVector += camera->getRigthVector() * moveSpeed;
    }

    if (openGlWrapper->getKeyStatus(GLFW_KEY_A) == GLFW_PRESS) {
        moveVector -= camera->getRigthVector() * moveSpeed;
    }

    if (glm::length(moveVector) > FLT_EPSILON) {
        camera->move(moveVector);
    }

    openGlWrapper->resetCursor();
}
