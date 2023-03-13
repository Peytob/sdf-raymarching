#ifndef SCENECONVERTIONEXCEPTION_HPP
#define SCENECONVERTIONEXCEPTION_HPP

#pragma once

#include <exception>
#include <string>

class SceneConvertionException : std::exception {
public:
    SceneConvertionException(const std::string& message) :
        message(message) {};

    const char* what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW override;

private:
    const std::string message;
};

#endif