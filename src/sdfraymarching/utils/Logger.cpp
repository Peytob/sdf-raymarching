#include <iostream>

#include "Logger.hpp"

void Logger::info(const std::string& message) {
    std::cout << message << std::endl;
}

void Logger::error(const std::string& message) {
    std::cerr << message << std::endl;
}
