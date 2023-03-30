#ifndef OPENGLWRAPPER_HPP
#define OPENGLWRAPPER_HPP

#pragma once

#include <string>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class Canvas;
class ShaderStorageBuffer;
class OpenGLRenderContext;
class Scene;
class ShaderProgram;

class OpenGLWrapper final {
public:
    OpenGLWrapper(int width, int height, const std::string& title);
    ~OpenGLWrapper();

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

    void updateDynamicUniforms(const OpenGLRenderContext& renderContext, ShaderProgram* shaderProgram);
    void updateStaticUniforms(const OpenGLRenderContext& renderContext, ShaderProgram* shaderProgram);

private:
    GLFWwindow* window;
    Canvas* canvas;

    glm::vec2 defaultCursorPosition;
};

#endif