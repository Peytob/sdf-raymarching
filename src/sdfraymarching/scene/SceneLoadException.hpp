#ifndef SCENELOADEXCEPTION_HPP
#define SCENELOADEXCEPTION_HPP

#pragma once

#include <exception>
#include <string>

class SceneLoadException : std::exception {
public:
    SceneLoadException(const std::string& message);

    const char* what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW override;

private:
    const std::string message;

};

#endif