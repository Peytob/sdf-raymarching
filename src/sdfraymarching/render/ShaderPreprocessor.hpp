#ifndef SHADERPREPROCESSOR_HPP
#define SHADERPREPROCESSOR_HPP

#pragma once

#include <string>

class Shader;

namespace ShaderPreprocessor {
    Shader loadShaderWithPreprocessor(const std::string& baseFolder, const std::string& filename, GLenum type);
};

#endif
