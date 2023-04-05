#include <regex>
#include <fstream>

#include <sdfraymarching/render/Shader.hpp>

#include "ShaderPreprocessor.hpp"

// TODO Защита от циркулярных ссылок; меморизация

namespace {

static const std::regex PRAGMA_REGEX = std::regex("^#pragma include \"(.+)\"");
std::string loadFileWithPreprocessor(const std::string& baseFolder, const std::string& filename, std::map<std::string, std::string>& loadedIncludes) {
    const std::string filePath = baseFolder + "/" + filename;

    std::ifstream file(filePath);

    std::string lineBuffer;
    std::string readedFileBuffer;

    while (std::getline(file, lineBuffer)) {
        std::smatch lineMatch;

        if (std::regex_match(lineBuffer, lineMatch, PRAGMA_REGEX)) {
            lineBuffer = loadFileWithPreprocessor(baseFolder, lineMatch[1], loadedIncludes);
        }

        readedFileBuffer += lineBuffer + "\n";
    }

    return readedFileBuffer;
}

};

Shader ShaderPreprocessor::loadShaderWithPreprocessor(const std::string& baseFolder, const std::string& filename, GLenum type, const std::map<std::string, std::string>& generatedIncludes) {
    std::map<std::string, std::string> loadedIncludes(generatedIncludes);

    const std::string shaderCode = loadFileWithPreprocessor(baseFolder, filename, loadedIncludes);
    return Shader(shaderCode, type);
}
