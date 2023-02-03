#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <string>

#pragma once

class OpenGLRender;
class ShaderProgram;
class Camera;
class Scene;

class Application {
public:
    Application();
    ~Application();

    int start();

private:
    OpenGLRender* renderer;
    ShaderProgram* worldShaderProgram;
    Scene* scene;

    Camera* camera;

    void processControl();
};

#endif