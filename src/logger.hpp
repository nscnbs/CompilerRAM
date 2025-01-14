#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>

class Logger {
private:
    std::ofstream log_file;

public:
    Logger(const std::string& file_name) {
        log_file.open(file_name, std::ios::app);
        if (!log_file.is_open()) {
            throw std::runtime_error("Error: Could not open log file: " + file_name);
        }
    }

    ~Logger() {
        if (log_file.is_open()) {
            log("_________________________________________________");
            log_file.close();
        }
    }

    void log(const std::string& message) {
        if (log_file.is_open()) {
            log_file << message << std::endl;
        }
    }
};

#endif // LOGGER_HPP
