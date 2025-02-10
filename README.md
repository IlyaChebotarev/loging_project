# Логгер сообщений

Этот проект включает библиотеку для записи сообщений в журнал и консольное приложение, которое демонстрирует её работу. Библиотека поддерживает три уровня важности сообщений: INFO, WARNING и ERROR.

## Особенности

- Запись сообщений в текстовый файл.
- Фильтрация сообщений по уровню важности.
- Многопоточная обработка сообщений.
- Простое консольное приложение для тестирования.

## Требования

- Компилятор `g++` (поддерживающий стандарт C++17).
- Операционная система: Linux (Ubuntu).

1)Сборка проекта

1.1) Клонируйте репозиторий:

   git clone https://github.com/IlyaChebotarev/loging_project.git
   cd loging_project
   
1.2)Соберите проект:

make
Это создаст:

Динамическую библиотеку liblogger.so.

Исполняемый файл my_app.

1.3)Очистите проект (удалите скомпилированные файлы):

make clean

2)Использование
2.1)Запуск приложения
Запустите приложение, указав файл журнала и уровень важности по умолчанию:

./my_app log.txt 0
log.txt — имя файла журнала.

0 — уровень важности по умолчанию (INFO).

3)Взаимодействие с приложением
3.1)Введите сообщение:

Введите сообщение: Это тестовое сообщение
3.2)Введите уровень важности:

Введите уровень важности (0 - INFO, 1 - WARNING, 2 - ERROR): 0
3.4)Для завершения работы введите exit.

4)Примеры
4.1)Запуск приложения:

./my_app log.txt 0
4.2)Ввод данных:

Введите сообщение: Приложение запущено
Введите уровень важности (0 - INFO, 1 - WARNING, 2 - ERROR): 0

Введите сообщение: Обнаружена ошибка
Введите уровень важности (0 - INFO, 1 - WARNING, 2 - ERROR): 2

Введите сообщение: exit
4.4)Проверка файла журнала:

cat log.txt
Вывод:

[2023-10-05 14:35:00] [INFO] Приложение запущено
[2023-10-05 14:35:15] [ERROR] Обнаружена ошибка
# loging_project
# test
# test
