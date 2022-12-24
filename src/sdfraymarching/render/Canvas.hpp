#ifndef CANVAS_HPP
#define CANVAS_HPP

#pragma once

#include <GL/gl.h>

class Canvas {
public:
    Canvas();
    ~Canvas();

    GLuint getVbo() const;
    GLuint getVao() const;
    GLuint getEbo() const;

    GLsizei getIndexesCount() const;

private:
    GLuint vbo;
    GLuint vao;
    GLuint ebo;
    GLsizei indexesCount;
};

#endif