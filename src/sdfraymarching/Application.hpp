#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <string>

#pragma once

class OpenGLWrapper;
class ShaderProgram;
class Camera;
class Scene;
class IDrawer;

class Application {
public:
    Application();
    ~Application();

    int start();

private:
    OpenGLWrapper* openGlWrapper;
    IDrawer* drawer;
    Scene* scene;

    Camera* camera;

    void processControl();
};

#endif