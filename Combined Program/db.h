#include "iostream"
#include "sqlite3.h"
using namespace std;
#ifndef UNTITLED_DB_H
#define UNTITLED_DB_H


bool executeQuery(sqlite3* db_name,const char* sql);
bool initializeDatabase(const string& db_name,sqlite3* db);



#endif
