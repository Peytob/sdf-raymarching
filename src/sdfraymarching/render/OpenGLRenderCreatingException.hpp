#ifndef OPENGLRENDERCREATINGEXCEPTION_HPP
#define OPENGLRENDERCREATINGEXCEPTION_HPP

#pragma once

#include <exception>
#include <string>

class OpenGLRenderCreatingException : std::exception {
public:
    OpenGLRenderCreatingException(std::string message);

    const char* what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW override;

private:
    const std::string message;
};

#endif