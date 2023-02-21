#include "SceneLoadException.hpp"

SceneLoadException::SceneLoadException(const std::string& message) :
    message(message) { }

const char* SceneLoadException::what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW {
    return message.c_str();
}
