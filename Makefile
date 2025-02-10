CC = g++
CFLAGS = -std=c++17 -Wall -Wextra -Iinclude

#цели по умолчанию 
all: liblogger.so my_app

#сборка библиотеки
liblogger.so: src/logger.cpp include/logger.h
	$(CC) $(CFLAGS) -shared -fPIC -o liblogger.so src/logger.cpp

#сборка приложения
my_app: app/main.cpp liblogger.so
	$(CC) $(CFLAGS) -o my_app app/main.cpp -L. -llogger -pthread -Wl,-rpath,.

#очистка
clean:
	rm -f *.so my_app
