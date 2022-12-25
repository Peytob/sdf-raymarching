#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#pragma once

#include <sdfraymarching/render/OpenGLRender.hpp>
#include <sdfraymarching/render/ShaderProgram.hpp>

class Application {
public:
    Application();
    ~Application();

    int start();

private:
    OpenGLRender* renderer;
    ShaderProgram* worldShaderProgram;

    void processControl();
};

#endif