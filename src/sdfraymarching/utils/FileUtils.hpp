#ifndef FILEUTILS_HPP
#define FILEUTILS_HPP

#pragma once

#include <string>

namespace FileUtils {
    std::string readFile(const std::string& fileName);
    std::pair<std::string, std::string> splitFolderAndFilename(const std::string& filepath);
};

#endif