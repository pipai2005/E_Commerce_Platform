#include"product.h"


Clothes::Clothes(int product_id, int business_id, string name, string description, double originPrice,
	double dicount_rate, int product_remain, int freeze_num, int type)
{
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

Clothes::~Clothes() {

}


double Clothes::getPrice() {
	return this->originPrice * this->dicount_rate;
}