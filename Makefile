
DB_LIB_PATH = db_module/ #путь до директории с файлом библиотеки и 
CFiles_OBG:
	gcc -c $(DB_LIB_PATH)sqlite3.c  -o $(DB_LIB_PATH)sqlite3.o 


build: CFiles_OBG 
	g++ -g sqlite3.h db_create_mod.cpp mainpr.cpp  sqlite3.o -lpthread -ldl 
	