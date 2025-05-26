#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <ctime>
#include <sstream>
#include <chrono>

class Logger {
public:
    static Logger& Instance() {
        static Logger instance;
        return instance;
    };

    template <typename... Args>
    void Log(const std::string& level, Args&&... messages) {
        std::string timestamp = getCurrentTime();
        out << "[" << timestamp << "](" << level << ") ";

        (out << ... << std::forward<Args>(messages));

        out << std::endl;
    }

    template <typename... Args>
    void Info(Args&&... messages) {
        Log("INFO", std::forward<Args>(messages)...);
    }

    template <typename... Args>
    void Warn(Args&&... messages) {
        Log("WARN", std::forward<Args>(messages)...);
    }

    template <typename... Args>
    void Error(Args&&... messages) {
        Log("ERROR", std::forward<Args>(messages)...);
    }
private:
    std::ofstream out;

    Logger() {
        out.open("log.txt", std::ios::out);
        if (!out.is_open()) {
            std::cerr << "Error: Failed to open log.txt" << std::endl;
        }
    }

    ~Logger() {
        out.close();
    }

    std::string getCurrentTime() {
        auto now = std::chrono::system_clock::now();
        time_t nowTimeT = std::chrono::system_clock::to_time_t(now);
        std::chrono::milliseconds nowMs = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
    
        std::ostringstream oss;
        oss << std::put_time(std::localtime(&nowTimeT), "%H:%M:%S") << '.' << std::setfill('0') << std::setw(3) << nowMs.count();
    
        return oss.str();
    }
};

#endif // LOGGER_H