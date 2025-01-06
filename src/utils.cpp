#include "utils.hpp"
#include <iostream>

void logError(const std::string& message) {
    std::cerr << "[ERROR]: " << message << std::endl;
}
