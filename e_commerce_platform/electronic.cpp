#include"product.h"


Electronic::Electronic(string name, string desc, double oP, double sP, int rem, bool isSpecial) {
	this->name = name;
	this->description = desc;
	this->orginPrice = oP;
	this->dicountRate = 1;
	this->remain = rem;
}

Electronic::~Electronic() {

}


double Electronic::getPrice() {
	return this->orginPrice * this->dicountRate;
}