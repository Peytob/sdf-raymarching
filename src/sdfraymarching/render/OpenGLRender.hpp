#ifndef OPENGLRENDER_HPP
#define OPENGLRENDER_HPP

#pragma once

#include <string>
#include <GLFW/glfw3.h>

class OpenGLRender final {
public:
    OpenGLRender(int width, int height, const std::string& title);
    ~OpenGLRender();

private:
    GLFWwindow* window;
};

#endif