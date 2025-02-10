#include "logger.h" //Подключение библиотеки Logger
#include <iostream>
#include <thread>
#include <queue>// Для очереди сообщений
#include <mutex>// Для синхронизации доступа к очереди
#include <condition_variable> //Для уведомления потоков об изменении состояния очереди
#include <limits>

using namespace std;

// Глобальная очередь для хранения сообщений журнала и их уровней важности
queue< pair< string, LogLevel>> logQueue;
// Мьютекс для защиты доступа к очереди
mutex queueMutex;
// Условная переменная для уведомления потока-обработчика о появлении новых сообщений в очереди
condition_variable queueCondition;
// Флаг для остановки потока-обработчика
bool stopLogging = false;

// Функция, которая будет выполняться в отдельном потоке для обработки сообщений из очереди
void logWorker(Logger& logger) {
    while (true) {
        pair< string, LogLevel> logEntry; // Переменная для хранения текущего сообщения и его уровня важности
        {
            unique_lock< mutex> lock(queueMutex);  // Блок, в котором мьютекс заблокирован. После выхода из блока мьютекс автоматически разблокируется.
            queueCondition.wait(lock, [] { return !logQueue.empty() || stopLogging; }); // Ждем, пока в очереди появятся сообщения или пока не будет установлен флаг остановки
            if (stopLogging && logQueue.empty()) break;
            logEntry = logQueue.front(); // Берем сообщение из начала очереди
            logQueue.pop(); // Удаляем сообщение из очереди
        }
        logger.log(logEntry.first, logEntry.second); //Запись в журнал
    }
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        cerr << "Использование: " << argv[0] << " <файл журнала> <уровень важности>" << endl;
        return 1;
    }

    Logger logger(argv[1], static_cast<LogLevel>(stoi(argv[2])));
    // Создаем и запускаем поток-обработчик сообщений из очереди.  Передаем ссылку на объект logger, чтобы поток мог использовать его для записи в журнал.
    thread worker(logWorker, ref(logger));

    string message;
    int level;
    while (true) { // Бесконечный цикл для приема сообщений от пользователя
        cout << "Введите сообщение: ";
        getline(cin, message);
        if (message == "exit") break;// Выход из цикла 

        while (true) { // Бесконечный цикл для запроса уровня важности, пока не будет введен корректный уровень
            cout << "Введите уровень важности (0 - INFO, 1 - WARNING, 2 - ERROR): ";
            cin >> level;

            if (cin.fail() || level < 0 || level > 2) { // Цикл для вывода ошибки
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cerr << "Ошибка: недопустимый уровень важности. Используйте 0 (INFO), 1 (WARNING), 2 (ERROR)." << endl;
                continue;
            }

            cin.ignore();
            break;
        }

        {
         // Блок, в котором мьютекс заблокирован для потокобезопасного доступа к очереди
            lock_guard< mutex> lock(queueMutex); // Автоматически блокирует и разблокирует мьютекс при входе и выходе из блока
            logQueue.push({message, static_cast<LogLevel>(level)});
        }
        queueCondition.notify_one(); // Уведомляем один из ожидающих потоков (поток-обработчик), что в очереди появились новые сообщения
    }

    stopLogging = true; // Устанавливаем флаг остановки потока-обработчика
    queueCondition.notify_one(); // Уведомляем поток-обработчик, что нужно завершить работу (даже если очередь пуста)
    worker.join();  // Ожидаем завершения работы потока-обработчика 

    return 0;
}
