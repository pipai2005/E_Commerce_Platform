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

void Product::showProductInfo(MySocket* mySocket, int level) {
	string buffer;
	buffer += "商品编号：" + to_string(product_id) + "\n";
	buffer += "商家编号：" + to_string(business_id) + "\n";
	buffer += "商品名称：" + name + "\n";
	if (level == 1) {
		buffer += "商品描述：" + description + "\n";
		buffer += "商品原价：" + to_string(originPrice) + "\n";
		buffer += "商品折扣率：" + to_string(dicount_rate) + "\n";
		buffer += "商品剩余量：" + to_string(product_remain) + "\n";
	}
	buffer += "商品类型：" + productType[type] + "\n";
	buffer += "\n";
	mySocket->sendData(buffer.c_str(), buffer.size());
}