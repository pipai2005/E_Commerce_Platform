#pragma once
// 电商管理系统

#include<iostream>
#include<vector>
#include<map>
#include<cstdlib>
#include<fstream>
#include<cassert>
#include<mysql.h>
#include <limits>  // 用于缓冲区清除限定

#include"Account.h"
#include"database_manager.h"

using namespace std;

#define ACCOUNT_FILENAME "accountTable.txt"



class ECManager {
public:

	ECManager();

	~ECManager();

	// 展示菜单
	void showOption();
	// 将账户文件中的数据读入对象
	void initAccount();
	// 查找账户文件中的对象 (返回值：-1：未找到 / 1：商家 / 2：消费者)
	bool searchAccount(const Account* ac) const;
	// 进行账户注册
	void registerAccount();
	// 进行账户登录
	void loginAccount();
	// 显示平台商品
	void showProduct();
	// 查找商品
	void searchProduct();
	// 退出商品平台
	void exitPlatform();
	// 将商家信息添加进入database
	void addAccountDatabase(const Account& ac);

	int product_num = 0;		//商品类型数
	int bussiness_num = 0;
	int consumer_num = 0;
	int account_num = 0;

	vector<Account*> accounts;
};