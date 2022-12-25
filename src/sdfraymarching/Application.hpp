#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#pragma once

class OpenGLRender;
class ShaderProgram;
class Camera;

class Application {
public:
    Application();
    ~Application();

    int start();

private:
    OpenGLRender* renderer;
    ShaderProgram* worldShaderProgram;
    Camera* camera;

    void processControl();
};

#endif