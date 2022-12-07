#ifndef LOGGER_HPP
#define LOGGER_HPP

#pragma once

#include <string>

namespace Logger {
    void info(const std::string& message);
    void error(const std::string& message);
}

#endif
