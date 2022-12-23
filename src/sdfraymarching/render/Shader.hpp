#ifndef SHADER_HPP
#define SHADER_HPP

#pragma once

#include <string>
#include <GL/gl.h>

class Shader final {
public:
    Shader(const std::string& code, GLenum type);
    ~Shader();

    GLenum getType() const;
    GLint getId() const;

private:
    const GLenum type;
    const GLint id;
};

#endif