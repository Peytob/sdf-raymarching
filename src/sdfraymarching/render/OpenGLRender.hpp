#ifndef OPENGLRENDER_HPP
#define OPENGLRENDER_HPP

#pragma once

#include <string>

#include <GLFW/glfw3.h>

#include <sdfraymarching/render/Canvas.hpp>

class OpenGLRenderContext;

class OpenGLRender final {
public:
    OpenGLRender(int width, int height, const std::string& title);
    ~OpenGLRender();

    void draw(const OpenGLRenderContext& renderContext);
    void pullEvents();
    void clear();
    void display();

private:
    GLFWwindow* window;
    Canvas* canvas;
};

#endif