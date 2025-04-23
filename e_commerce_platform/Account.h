#pragma once
#include<iostream>
#include<string>
#include<vector>
#include<map>
#include<fstream>
#include<cassert>

#include"product.h"
#include"database_manager.h"


#define ACCOUNT_FILENAME "accountTable.txt"

class Account {
public:
	Account() : id(0), balance(0), type(0) {} // 默认构造函数

	// 账户界面选项
	virtual void showAccountOption() = 0;
	virtual void selectOption() = 0;

	// 显示账户类型
	virtual string getAccountType() = 0;
	// 修改密码
	void updatePassword();
	// 显示商品每一页 10
	void displayPage(Database* db, int pageNumber);
	// 显示平台商品
	void showProduct();
	// 查找商品
	void searchProduct();
	// 显示账户信息
	void showAccount();
	// 余额查询
	void getBalance();
	// 余额充值
	void paying();
	// 余额消费
	void consuming(double e);

	//查找(失败返回0)
	void showProductInfoByName(Database* db, string line_name);
	int searchProductByName();
	int searchProductByType();


	int id;					//序号
	string name;			//账户名
	string password;		//密码
	double balance;		//账户余额
	int type;			//账户类型：1：bussiness / 2: consumer
};

class Bussiness : public Account {
public:
	Bussiness(int no, string name, string p, double b, int t);

	~Bussiness();

	virtual string getAccountType();


	// 商家操作
	// 商家界面显示
	void showAccountOption();
	void selectOption();
	// 商品打折
	void discount();
	// 添加商品入库
	void addProductStock();
	// 查看库存
	void showStock();
	// 商家属性

};

class Consumer : public Account {
public:
	Consumer(int no, string id,string p,double b,int t);

	~Consumer();
	
	virtual string getAccountType();

	// 消费者操作
	// 消费者界面显示
	void showAccountOption();
	void selectOption();
	//购物车添加
	void addShopCart();
	//购物车删除
	void delShopCart();
	//修改购物车拟购买商品数量
	void updateShopCartProductNum();
	//生成订单
	void generateOrder();
};

