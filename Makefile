
DB_LIB_PATH = db_module/ #путь до директории с файлом библиотеки и 
DB_FILES_TO_COMP = $(DB_LIB_PATH)db_create_mod.h $(DB_LIB_PATH)db_create_mod.cpp $(DB_LIB_PATH)sqlite3.h $(DB_LIB_PATH)sqlite3.o 
CFiles_OBG: # создает объектные файлы библиотеки sqlite3
	gcc -c $(DB_LIB_PATH)sqlite3.c  -o $(DB_LIB_PATH)sqlite3.o 

build: #CFiles_OBG когда начнешь использовать бд, добавь $(DB_FILES_TO_COMP)
	g++ telegram_bot.cpp -o telegram_bot --std=c++14 -I/usr/local/include -lTgBot -lboost_system -lssl -lcrypto -lpthread -lcurl
	