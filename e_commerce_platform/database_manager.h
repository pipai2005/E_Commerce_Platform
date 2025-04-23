#pragma once
#include<mysql.h>
#include<iostream>
#include<string>

#define DB_PSW "Hua20050924"
using namespace std;

class Database {
public:

    MYSQL mysql;
    string op;              // 操作执行语句

    Database();
    ~Database();

    void getConnect();
    void query();
    // 获取表格行数
	int getResultNum();
};
