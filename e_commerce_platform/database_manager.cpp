#include"database_manager.h"


Database::Database(){

    // 初始化数据库连接，获取句柄
    mysql_init(&mysql);

}

Database::~Database() {
    // 释放数据库连接资源
    mysql_free_result(result);
    mysql_close(&mysql);
}

void Database::getConnect() {

    if (!mysql_real_connect(&mysql, "localhost", "root", DB_PSW,
        "products", 3306, NULL, 0)) {
        cout << "数据库连接失败" << endl;
        exit(1);
    }
    cout << "数据库连接成功" << endl;
}

void Database::query() {
    if (op.size() == 0) {
        cout << "mysql 操作语句未填写" << endl;
        return;
    }
   

    if (mysql_query(&this->mysql, this->op.c_str())) {
        cout << "查询失败" << endl;
        return;
    }

    result = mysql_store_result(&this->mysql);
    if (result == NULL) {
        std::cerr << "mysql_store_result() failed: " << mysql_error(&mysql) << std::endl;
        return;
    }
}