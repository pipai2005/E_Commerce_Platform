#pragma once
// 电商管理系统 ---- 客户端

#include<iostream>
#include<vector>
#include<map>
#include<string>
#include<cstdlib>
#include<fstream>
#include<cassert>
#include <limits>  // 用于缓冲区清除限定

#include"../client_e_commerce/tcp_socket.h"
#include"../client_e_commerce/AccountWidget.h"
using namespace std;

#define ACCOUNT_FILENAME "accountTable.txt"
//#define SIO_UDP_CONNRESET _WSAIOW(IOC_VENDOR, 12)

// 电商管理系统客户端类 主界面
class ECManager {
public:

	ECManager();

	~ECManager();

	// 展示菜单
	void showOption();
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

	MySocket* mySocket = NULL;  // 客户端套接字(记录当前连接的套接字)
	Account* account = NULL;    // 登录账户
};