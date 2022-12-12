#include "OpenGLRenderCreatingException.hpp"

OpenGLRenderCreatingException::OpenGLRenderCreatingException(std::string message) :
    message(message) { }

const char* OpenGLRenderCreatingException::what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW {
    return message.c_str();
}
