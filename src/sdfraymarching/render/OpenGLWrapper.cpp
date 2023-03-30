#include <string>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <sdfraymarching/utils/Logger.hpp>
#include <sdfraymarching/render/Canvas.hpp>
#include <sdfraymarching/render/ShaderStorageBuffer.hpp>
#include <sdfraymarching/render/OpenGLRenderCreatingException.hpp>
#include <sdfraymarching/render/OpenGLRenderContext.hpp>
#include <sdfraymarching/render/ShaderProgram.hpp>
#include <sdfraymarching/render/Camera.hpp>
#include <sdfraymarching/scene/SceneToPlainConverter.hpp>

#include <sdfraymarching/scene/Scene.hpp>

#include "OpenGLWrapper.hpp"

void initializeGlfw() {
    if (glfwInit() != GLFW_TRUE) {
        const char* errorDescription;
        const int errorCode = glfwGetError(&errorDescription);

        std::string message =
            "Error while GLFW initializating! Code: " + std::to_string(errorCode) +
            "; description: " + std::string(errorDescription);

        throw OpenGLRenderCreatingException(message);
    }
}

GLFWwindow* createWindow(int width, int height, const std::string& title) {
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow* window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);

    if (nullptr == window) {
        const char* description;
        glfwGetError(&description);

        std::string message = "Error while creating GLFW window. " + std::string(description);

        throw OpenGLRenderCreatingException(message);
    }

    return window;
}

void initializeGlew() {
    glewExperimental = GL_TRUE;
    const GLenum glewInitializationCode = glewInit();
    if (glewInitializationCode != GLEW_OK) {
        std::string message =
            "Error while GLEW initialzing!. "
            + std::string((const char*) glewGetErrorString(glewInitializationCode));

        throw OpenGLRenderCreatingException(message);
    }
}

OpenGLWrapper::OpenGLWrapper(int width, int height, const std::string& title) :
    window(nullptr),
    defaultCursorPosition(width / 2, height / 2) {

    Logger::info("Initializing OpenGL rendering engine.");

    Logger::info("Initializing GLFW library.");
    initializeGlfw();
    Logger::info("GLFW library initialized.");

    Logger::info("Creating new GLFW window.");
    this->window = createWindow(width, height, title);
    glfwMakeContextCurrent(window);
    glfwSetWindowUserPointer(window, this);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    Logger::info("New GLFW window created.");

    Logger::info("Initializing GLEW library.");
    initializeGlew();
    Logger::info("GLEW library initialized.");

    this->canvas = new Canvas();
}

OpenGLWrapper::~OpenGLWrapper() {
    Logger::info("Destroying OpenGL rendering engine.");

    Logger::info("Destroying GLFW window");
    glfwDestroyWindow(window);
    Logger::info("GLFW window destroyed.");

    Logger::info("Terminating GLFW library.");
    glfwTerminate();
    Logger::info("GLFW library terminated.");

    delete canvas;
}

glm::vec2 OpenGLWrapper::getCursorDelta() const {
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    return glm::vec2(xpos, ypos) - defaultCursorPosition;
}

bool OpenGLWrapper::isClosed() {
    return glfwWindowShouldClose(window);
}

void OpenGLWrapper::close() {
    glfwSetWindowShouldClose(window, true);
}

void OpenGLWrapper::setKeyCallback(GLFWkeyfun callback) {
    Logger::info("Setup window key callback");
    glfwSetKeyCallback(window, callback);
}

glm::ivec2 OpenGLWrapper::getResolution() const {
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    return { width, height };
}

void OpenGLWrapper::draw(const OpenGLRenderContext& renderContext) {
    glBindVertexArray(canvas->getVao());
    glDrawElements(GL_TRIANGLES, canvas->getIndexesCount(), GL_UNSIGNED_INT, 0);
}

void OpenGLWrapper::resetCursor() {
    glfwSetCursorPos(window, defaultCursorPosition.x, defaultCursorPosition.y);
}

void OpenGLWrapper::pullEvents() {
    glfwPollEvents();
}

void OpenGLWrapper::clear() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void OpenGLWrapper::display() {
    glfwSwapBuffers(window);
}

int OpenGLWrapper::getKeyStatus(int key) {
    return glfwGetKey(window, key);
}

void OpenGLWrapper::updateDynamicUniforms(const OpenGLRenderContext& renderContext, ShaderProgram* shaderProgram) {
    Camera* camera = renderContext.getCamera();
    shaderProgram->setUniform("u_view", camera->computeLookAtMatrix());
    shaderProgram->setUniform("u_cameraPosition", camera->getPosition());
}

void OpenGLWrapper::updateStaticUniforms(const OpenGLRenderContext& renderContext, ShaderProgram* shaderProgram) {
    shaderProgram->setUniform("u_resolution", renderContext.getResolution());
}
