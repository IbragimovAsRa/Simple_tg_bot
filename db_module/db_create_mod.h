#ifndef _DB_CREATE_MOD_H
#define _DB_CREATE_MOD_H
#include <iostream>
#include "sqlite3.h"
#include <string>
#include <cstdlib>

using namespace std;
class Data{
    public:
        int id;
        double trans;
        string date[15];
        string inform[20];
        int col_num = 4;
};
// создает или просто открывает таблицу. На вход подается
// бд, имя таблицы, количество колонок и массив названий колонок стринговый
// названия должны быть как в SQLlite, так что почитай или напиши мне
int create_tabl(sqlite3* DB, string tabl_name, int col_num, string col_names[]);
// добавляет элемент. Последний параметр это стринговый массив, где каждый
//элемент соответствует своей колонке
int insert_elem(sqlite3* DB, string tabl_name, int col_num, string row_values[]);
// удаляет все элементы из таблицы, для которых верно условие.
// сиснтаксис условия смотри в докум. SQLite или у меня спроси
int delete_elem(sqlite3* DB, string tabname, string conclusion);
// удаляет все элементы таблицы
int clear_tabl(sqlite3* DB, string tabname);
// удаляет таблицу
int delete_tabl(sqlite3* DB, string tabname);
// 
int sum_col_last_mon(sqlite3* DB, string tabname, string colname,int id, int from_month, int to_month);

void close_db(sqlite3* DB);

#endif