#include <GL/glew.h>
#include <sdfraymarching/utils/Logger.hpp>

#include "ShaderStorageBuffer.hpp"

ShaderStorageBuffer::ShaderStorageBuffer(GLuint bindingIndex) :
    bindingIndex(bindingIndex) {
    Logger::info("Creating new shader storage buffer");
    glGenBuffers(1, &id);
    Logger::info("Created new shader storage buffer with id " + std::to_string(id));
}

ShaderStorageBuffer::~ShaderStorageBuffer() {
    Logger::info("Deleting shader storage buffer with id " + std::to_string(id));
    glDeleteBuffers(1, &id);
}

void ShaderStorageBuffer::writeData(GLsizeiptr size, void* data, GLenum usage) {
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, id);
    glBufferData(GL_SHADER_STORAGE_BUFFER, size, data, usage);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, bindingIndex, id);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

GLuint ShaderStorageBuffer::getId() const {
    return id;
}

GLuint ShaderStorageBuffer::getBindingIndex() const {
    return bindingIndex;
}
