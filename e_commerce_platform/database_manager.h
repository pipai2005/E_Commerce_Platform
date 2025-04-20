#pragma once
#include<mysql.h>
#include<iostream>
#include<string>

using namespace std;

#define DB_PSW "Hua20050924"

class Database {
public:

    MYSQL mysql;
    string op;              // 操作执行语句
    MYSQL_RES* result;

    Database();
    ~Database();

    void getConnect();
    void query();
};
