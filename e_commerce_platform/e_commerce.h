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
#include"../e_commerce_platform/tcp_socket.h"


#define ACCOUNT_FILENAME "accountTable.txt"
//#define SIO_UDP_CONNRESET _WSAIOW(IOC_VENDOR, 12)


class ECManager {
public:

	ECManager();

	~ECManager();

	// 展示菜单
	void showOption();
	// 将账户文件中的数据读入对象
	void initAccount();
	// 进行账户注册
	void registerAccount();
	// 进行账户登录
	void loginAccount();
	// 显示平台商品
	void displayPage(Database* db, int passageNum);
	void showProduct();
	// 查找商品
	void searchProduct();
	int searchProductByName();
	void showProductInfoByName(Database* db, string line_name);
	// 退出商品平台
	void exitPlatform();
	// 将商家信息添加进入database
	void addAccountDatabase(const Account& ac);

	int product_num = 0;		//商品类型数
	int bussiness_num = 0;
	int consumer_num = 0;
	int account_num = 0;

	MySocket* mySocket;         // 用于网络通信的套接字(当前登录的用户)
	vector<Account*> accounts;
};