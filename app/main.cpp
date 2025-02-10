#include "logger.h"
#include <iostream>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <limits>

std::queue<std::pair<std::string, LogLevel>> logQueue;
std::mutex queueMutex;
std::condition_variable queueCondition;
bool stopLogging = false;

void logWorker(Logger& logger) {
    while (true) {
        std::pair<std::string, LogLevel> logEntry;
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            queueCondition.wait(lock, [] { return !logQueue.empty() || stopLogging; });
            if (stopLogging && logQueue.empty()) break;
            logEntry = logQueue.front();
            logQueue.pop();
        }
        logger.log(logEntry.first, logEntry.second);
    }
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Использование: " << argv[0] << " <файл журнала> <уровень важности>" << std::endl;
        return 1;
    }

    Logger logger(argv[1], static_cast<LogLevel>(std::stoi(argv[2])));
    std::thread worker(logWorker, std::ref(logger));

    std::string message;
    int level;
    while (true) {
        std::cout << "Введите сообщение: ";
        std::getline(std::cin, message);
        if (message == "exit") break;

        while (true) {
            std::cout << "Введите уровень важности (0 - INFO, 1 - WARNING, 2 - ERROR): ";
            std::cin >> level;

            if (std::cin.fail() || level < 0 || level > 2) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cerr << "Ошибка: недопустимый уровень важности. Используйте 0 (INFO), 1 (WARNING), 2 (ERROR)." << std::endl;
                continue;
            }

            std::cin.ignore();
            break;
        }

        {
            std::lock_guard<std::mutex> lock(queueMutex);
            logQueue.push({message, static_cast<LogLevel>(level)});
        }
        queueCondition.notify_one();
    }

    stopLogging = true;
    queueCondition.notify_one();
    worker.join();

    return 0;
}
