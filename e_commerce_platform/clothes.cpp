#include"product.h"


Clothes::Clothes(string name, string desc, double oP, double sP, int rem, bool isSpecial) {
	this->name = name;
	this->description = desc;
	this->orginPrice = oP;
	this->dicountRate = 1;
	this->remain = rem;
}

Clothes::~Clothes() {

}


double Clothes::getPrice() {
	return this->orginPrice * this->dicountRate;
}