#include "OpenGLResourceCreatingException.hpp"

OpenGLResourceCreatingException::OpenGLResourceCreatingException(std::string message) :
    message(message) { }

const char* OpenGLResourceCreatingException::what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW {
    return message.c_str();
}