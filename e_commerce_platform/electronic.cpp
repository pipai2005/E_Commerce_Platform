#include"product.h"


Electronic::Electronic(int product_id, int business_id, string name, string description, double originPrice,
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


Electronic::~Electronic() {

}


double Electronic::getPrice() {
	return this->originPrice * this->dicount_rate;
}