#include "db_create_mod.h"
using namespace std;
/*
В библиотеке для c/c++ есть только три нужных нам функции:
-sqlite3_open(<путь>,sqlite3* DB) - связь между переменной и бд
создает бд, если её нет
-sqlite3_close(DB); - после завершения работы с бд
nt sqlite3_exec( sqlite3* DB, const char* query, callback, void* udp, char** errmsg );
эту долго описывать, почитаета сами) она получает строку query в которой 
написан код на языке SQLlite и выполняет действие с бд. 

функция sum_callb(void* res, int col_num, char** row_val, char** col_name)
применяется последовательно к каждой строчке подаваемой на вход таблицы 

*/
static int sum_callb(void* res, int col_num, char** row_val, char** col_name){
    if (row_val[0]== NULL) {
        *((double*) res) = 0;
    }else{
        *((double*) res) = stod(row_val[0]);
    }
    return 0;
}
// на вход подается бд, путь, кол-во колонок и массив названий колонок
int create_tabl(sqlite3* DB, string tabl_name, int col_num, string col_names[]){
    char* messaggeError;

    string query = "CREATE TABLE IF NOT EXISTS "+ tabl_name +"( ";
    for (int i=0; i<col_num-1; ++i){
        query+=col_names[i];
        query+=", ";
    }
    query+=col_names[col_num-1];
    query += ");";
  int exit = sqlite3_exec(DB, query.c_str(), NULL, NULL, &messaggeError);
   if (exit != SQLITE_OK) {
        std::cerr << "Error Create" << std::endl;
        sqlite3_free(messaggeError);
        cout << query << endl;
    }
    return 0;
} 
void close_db(sqlite3* DB){
    sqlite3_close(DB);
}

int insert_elem(sqlite3* DB, string tabl_name, int col_num, string row_values[]){
    string  query = "INSERT INTO "+ tabl_name +" VALUES(";
    for (int i=0; i<col_num-1; ++i){
        query+=row_values[i];
        query+=", ";
    }
    query+=row_values[col_num-1];
    query += ");";
    char* messaggeError;
    int exit = sqlite3_exec(DB, query.c_str(), NULL, NULL, &messaggeError);
    if (exit != SQLITE_OK) {
        std::cerr << "Error Insert" << std::endl;
        sqlite3_free(messaggeError);
        cout << query << endl;
    }

    return 0;
}

int delete_elem(sqlite3* DB, string tabname, string conclusion){
    string query = "DELETE FROM " + tabname +" WHERE " + conclusion +" ;";
    char* messaggeError;
    int exit = sqlite3_exec(DB, query.c_str(), NULL, NULL, &messaggeError);
    if (exit != SQLITE_OK) {
        std::cerr << "Error Delete Elem" << std::endl;
        sqlite3_free(messaggeError);
        cout << query << endl;
    }
    return 0;
}

int clear_tabl(sqlite3* DB, string tabname){
    string query = "DELETE FROM " + tabname +";";
    char* messaggeError;
    int exit = sqlite3_exec(DB, query.c_str(), NULL, NULL, &messaggeError);
    if (exit != SQLITE_OK) {
        std::cerr << "Error Clear" << std::endl;
        sqlite3_free(messaggeError);
        cout << query << endl;
    }
    return 0;
}

int delete_tabl(sqlite3* DB, string tabname){
    string query = "DROP TABLE IF EXISTS "+ tabname+";";
    char* messaggeError;
    int exit = sqlite3_exec(DB, query.c_str(), NULL, NULL, &messaggeError);
    if (exit != SQLITE_OK) {
        std::cerr << "Error Delete Tabl" << std::endl;
        sqlite3_free(messaggeError);
    }
    return 0;
}

double sum_col_last_mon(sqlite3* DB, string tabname, string colname,int id, int from_month, int to_month){
    string query = "SELECT SUM("+ colname +") FROM " + tabname +\
    " WHERE id = "+to_string(id)+ " AND date BETWEEN date('now', 'start of month', '-"+to_string(from_month)+" month' )";
    if (to_month == 0){
       query +=" AND date('now');"; 
    }else{
       query +=" AND date('now', 'start of month', '-"+to_string(to_month)+" month' )";
    }
    char* messaggeError;
    double res = 0;
    int exit = sqlite3_exec(DB, query.c_str(), sum_callb, &res, &messaggeError);
   cout<<"Cумма за прошедщие месяцы = "<< res<<endl;
    if (exit != SQLITE_OK) {
        std::cerr << "Error Sum" << std::endl;
        cout << query << endl;
        sqlite3_free(messaggeError);
    }
    return res;
}

//int sqlite3_data_count(sqlite3_stmt *pStmt); - количество колонок

