#include <fstream>
#include <sstream>

#include "FileUtils.hpp"

std::string FileUtils::readFile(const std::string& fileName) {
    std::ifstream fileStream(fileName);
    std::stringstream buffer;
    buffer << fileStream.rdbuf();
    return buffer.str();
}

std::pair<std::string, std::string> FileUtils::splitFolderAndFilename(const std::string& filepath) {
    size_t lastSlashIndex = filepath.find_last_of("/\\");
    if (lastSlashIndex == filepath.npos) {
        return { "", filepath };
    } else {
        return { filepath.substr(0, lastSlashIndex), filepath.substr(lastSlashIndex + 1) };
    }
}
