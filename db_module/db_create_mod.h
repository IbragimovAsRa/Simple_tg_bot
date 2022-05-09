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
int create_tabl(sqlite3* DB, string tabl_name, int col_num, string col_names[]);
int insert_elem(sqlite3* DB, string tabl_name, int col_num, string row_values[]);
int delete_elem(sqlite3* DB, string tabname, string conclusion);
int clear_tabl(sqlite3* DB, string tabname);
int delete_tabl(sqlite3* DB, string tabname);
int sum_col_last_mon(sqlite3* DB, string tabname, string colname,int id, int from_month, int to_month);
void close_db(sqlite3* DB);

#endif