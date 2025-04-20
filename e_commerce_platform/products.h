#pragma once
#include<iostream>
#include<string>

#include "database_manager.h"

class Product {
public:
	Product() {
		this->name.reserve(50);
		this->description.reserve(255);
		this->type = 1;
	}

	
	int type	;						//商品类型
	string name;						//名字
	string description;					//描述
	double originPrice = 0;				//原价
	unsigned int purchaseNum = 0;		//进货量
} ;