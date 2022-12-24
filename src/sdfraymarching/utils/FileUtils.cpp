#include <fstream>
#include <sstream>

#include "FileUtils.hpp"

std::string FileUtils::readFile(const std::string& fileName) {
    std::ifstream fileStream(fileName);
    std::stringstream buffer;
    buffer << fileStream.rdbuf();
    return buffer.str();
}
