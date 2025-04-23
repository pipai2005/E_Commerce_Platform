#include"product.h"

extern map<int, string> productType;

Product::Product() {
	this->product_id = 0;			
	this->business_id = 0;				
	this->originPrice = 0;				
	this->dicount_rate = 0;			
	this->product_remain = 0;			
	this->freeze_num = 0;				
	this->type = 0;					
}

Product::Product(int product_id, int business_id, string name, string description, double originPrice,
	double dicount_rate, int product_remain, int freeze_num, int type) {
	this->product_id = product_id;
	this->business_id = business_id;
	this->name = name;
	this->description = description;
	this->originPrice = originPrice;
	this->dicount_rate = dicount_rate;
	this->product_remain = product_remain;
	this->freeze_num = freeze_num;
	this->type = type;	
}

void Product::showProductInfo() {
	cout << "商品编号：" << product_id << endl;
	cout << "商家序号：" << business_id << endl;
	cout << "商品名称：" << name << endl;
	cout << "商品描述：" << description << endl;
	cout << "商品原价：" << originPrice << endl;
	cout << "商品折扣率：" << dicount_rate << endl;
	cout << "商品类型：" << productType[type] << endl;
	cout << endl;
}