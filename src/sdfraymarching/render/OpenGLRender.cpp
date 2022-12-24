#include <string>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <sdfraymarching/utils/Logger.hpp>
#include <sdfraymarching/render/Canvas.hpp>
#include <sdfraymarching/render/OpenGLRenderCreatingException.hpp>
#include <sdfraymarching/render/OpenGLRenderContext.hpp>

#include "OpenGLRender.hpp"

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
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
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

OpenGLRender::OpenGLRender(int width, int height, const std::string& title) :
    window(nullptr) {

    Logger::info("Initializing OpenGL rendering engine.");

    Logger::info("Initializing GLFW library.");
    initializeGlfw();
    Logger::info("GLFW library initialized.");

    Logger::info("Creating new GLFW window.");
    this->window = createWindow(width, height, title);
    glfwMakeContextCurrent(window);
    Logger::info("New GLFW window created.");

    Logger::info("Initializing GLEW library.");
    initializeGlew();
    Logger::info("GLEW library initialized.");

    canvas = new Canvas();
}

OpenGLRender::~OpenGLRender() {
    Logger::info("Destroying OpenGL rendering engine.");

    Logger::info("Destroying GLFW window");
    glfwDestroyWindow(window);
    Logger::info("GLFW window destroyed.");

    Logger::info("Terminating GLFW library.");
    glfwTerminate();
    Logger::info("GLFW library terminated.");

    delete canvas;
}

void OpenGLRender::draw(const OpenGLRenderContext& renderContext) {
    glUseProgram(renderContext.getShaderProgram()->getId());
    glBindVertexArray(canvas->getVao());
    glDrawElements(GL_TRIANGLES, canvas->getIndexesCount(), GL_UNSIGNED_INT, 0);
}

void OpenGLRender::pullEvents() {
    glfwPollEvents();
}

void OpenGLRender::clear() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void OpenGLRender::display() {
    glfwSwapBuffers(window);
}
