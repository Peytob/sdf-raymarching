#ifndef SHADERPREPROCESSOR_HPP
#define SHADERPREPROCESSOR_HPP

#pragma once

#include <string>
#include <map>

class Shader;

namespace ShaderPreprocessor {
    Shader loadShaderWithPreprocessor(const std::string& baseFolder, const std::string& filename, GLenum type, const std::map<std::string, std::string>& generatedIncludes);
};

#endif
