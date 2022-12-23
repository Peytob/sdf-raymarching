#ifndef OPENGLRESOURCECREATINGEXCEPTION_HPP
#define OPENGLRESOURCECREATINGEXCEPTION_HPP

#pragma once

#include <string>

class OpenGLResourceCreatingException : std::exception {
public:
    OpenGLResourceCreatingException(std::string message);

    const char* what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW override;

private:
    const std::string message;
};

#endif