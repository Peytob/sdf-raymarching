#include "SceneConvertionException.hpp"

const char* SceneConvertionException::what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW {
    return message.c_str();
}
