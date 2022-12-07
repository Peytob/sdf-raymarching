#include <string>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <sdfraymarching/utils/Logger.hpp>

#include "OpenGLRender.hpp"

OpenGLRender::OpenGLRender() {
    Logger::info("Initializing OpenGL rendering engine.");

    Logger::info("Initializing GLFW library.");
    if (glfwInit() != GLFW_TRUE) {
        const char* errorDescription;
        const int errorCode = glfwGetError(&errorDescription);

        std::string message =
            "Error while GLFW initializating! Code: " + std::to_string(errorCode) +
            "; description: " + std::string(errorDescription);

        Logger::error(message);
    }
    Logger::info("GLFW library was initialized.");
}

OpenGLRender::~OpenGLRender() {
    Logger::info("Destroying OpenGL rendering engine.");
}
