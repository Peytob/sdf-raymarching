#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#pragma once

#include "render/OpenGLRender.hpp"

class Application {
public:
    Application();
    ~Application();

private:
    OpenGLRender* renderer;
};

#endif