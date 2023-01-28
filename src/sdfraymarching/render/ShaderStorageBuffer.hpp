#ifndef SHADERSTORAGEBUFFER_HPP
#define SHADERSTORAGEBUFFER_HPP

#include <GL/gl.h>

#pragma once

class ShaderStorageBuffer final {
public:
    ShaderStorageBuffer(GLuint bindingIndex);
    ~ShaderStorageBuffer();

    void writeData(GLsizeiptr size, void* data, GLenum usage);

    GLuint getId() const;
    GLuint getBindingIndex() const;

private:
    GLuint id;
    const GLuint bindingIndex;
};

#endif
