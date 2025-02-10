#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <fstream>
#include <mutex>

enum LogLevel {
   INFO,
   WARNING,
   ERROR
};

class Logger {
public:
   Logger(const std::string& filename, LogLevel defultLevel = INFO);
   void log(const std::string& message, LogLevel level = INFO);
   void setLogLevel(LogLevel level);
   
private:
    std::ofstream logFile;
    LogLevel currentLevel;
    std::mutex mtx;
    std::string getCurrentTime();
};

#endif 
