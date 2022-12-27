#include <GL/glew.h>

#include <sdfraymarching/utils/Logger.hpp>

#include "Canvas.hpp"

Canvas::Canvas() {
    Logger::info("Creating new canvas");

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    Logger::info("Created canvas buffers. Ids: VBO=" + std::to_string(vbo)
        + "; EBO=" + std::to_string(ebo) + "; VAO=" + std::to_string(vao));

    glBindVertexArray(vao);

    GLfloat vertices[] = {
         1.0f,  1.0f, 0.0f,
         1.0f, -1.0f, 0.0f,
        -1.0f, -1.0f, 0.0f,
        -1.0f,  1.0f, 0.0f
    };

    GLuint indices[] = {
        0, 1, 3,
        1, 2, 3
    };

    indexesCount = sizeof(indices) / sizeof(GLuint);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

Canvas::~Canvas() {
    Logger::info("Deleting canvas resources. Ids: VBO=" + std::to_string(vbo)
        + "; EBO=" + std::to_string(ebo) + "; VAO=" + std::to_string(vao));

    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
}

GLuint Canvas::getEbo() const {
    return vbo;
}

GLuint Canvas::getVao() const {
    return vao;
}

GLuint Canvas::getVbo() const {
    return vbo;
}

GLsizei Canvas::getIndexesCount() const {
    return indexesCount;
}
