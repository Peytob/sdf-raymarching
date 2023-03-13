#include "SceneLoadException.hpp"

const char* SceneLoadException::what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW {
    return message.c_str();
}
