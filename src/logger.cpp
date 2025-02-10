#include "logger.h"
#include <iomanip>
#include <ctime>

Logger::Logger(const std::string& filename, LogLevel defaultLevel)
    : currentLevel(defaultLevel) {
    logFile.open(filename, std::ios::app);
}

void Logger::log(const std::string& message, LogLevel level) {
    if (level < currentLevel) return;
    
    std::lock_guard<std::mutex> lock(mtx);
    logFile << "[" << getCurrentTime() << "]"
            << "[" << (level == INFO ? "INFO" : level == WARNING ? "WARNING" : "ERROR") << "]"
            <<message << std::endl;
}

void Logger::setLogLevel(LogLevel level) {
     currentLevel = level;
}

std::string Logger::getCurrentTime() {
     auto now = std::time(nullptr);
     auto tm = *std::localtime(&now);
     std::ostringstream oss;
     oss << std::put_time(&tm, "%y-%m-%d %H:%M:%s");
     return oss.str();
}
