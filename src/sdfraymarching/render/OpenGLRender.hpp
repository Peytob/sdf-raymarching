#ifndef OPENGLRENDER_HPP
#define OPENGLRENDER_HPP

#pragma once

#include <string>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <sdfraymarching/render/Canvas.hpp>

class OpenGLRenderContext;

class OpenGLRender final {
public:
    OpenGLRender(int width, int height, const std::string& title);
    ~OpenGLRender();

    void resetCursor();
    void draw(const OpenGLRenderContext& renderContext);
    void pullEvents();
    void clear();
    void display();

    void close();
    bool isClosed();

    glm::vec2 getCursorDelta() const;
    int getKeyStatus(int key);
    void setKeyCallback(GLFWkeyfun callback);

    glm::ivec2 getResolution() const;

    void updateDynamicUniforms(const OpenGLRenderContext& renderContext);
    void updateStaticUniforms(const OpenGLRenderContext& renderContext);

private:
    GLFWwindow* window;
    Canvas* canvas;

    glm::vec2 defaultCursorPosition;
};

#endif