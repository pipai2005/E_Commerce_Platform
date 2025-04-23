#pragma once
#include<iostream>
#include<mysql.h>
#include<map>
#include"database_manager.h"

using namespace std;


class Product {
public:

	Product();
	Product(int product_id, int business_id, string name, string description, double originPrice,
		double dicount_rate, int product_remain, int freeze_num, int type);

	virtual double getPrice() = 0;

	void showProductInfo();

	int product_id;					    //商品序号
	int business_id;					//商家序号
	string name;						//商品名称
	string description;					//商品描述
	double originPrice;					//商品原价
	double dicount_rate;				//商品折扣率
	int product_remain;					//商品剩余量
	int freeze_num;						//商家冻结商品数
	int type;							//商品类型
	
};

class Book : public Product {
public:
	Book(int product_id, int business_id, string name, string description, double originPrice,
		double dicount_rate, int product_remain, int freeze_num, int type);
	~Book();

	double getPrice();
};

class Clothes : public Product {
public:
	Clothes(int product_id, int business_id, string name, string description, double originPrice,
		double dicount_rate, int product_remain, int freeze_num, int type);
	~Clothes();

	double getPrice();
};

class Electronic : public Product {
public:
	Electronic(int product_id, int business_id, string name, string description, double originPrice,
		double dicount_rate, int product_remain, int freeze_num, int type);
	~Electronic();
	double getPrice();
};

class Food : public Product {
public:
	Food(int product_id, int business_id, string name, string description, double originPrice,
		double dicount_rate, int product_remain, int freeze_num, int type);
	~Food();

	double getPrice();
};