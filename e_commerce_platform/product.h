#pragma once
#include<iostream>
#include<mysql.h>

using namespace std;

class Product {
public:

	virtual double getPrice() = 0;

	string name;
	string description;
	double orginPrice;
	double dicountRate;
	int remain;
};

class Book : public Product {
public:
	Book(string name, string desc, double oP, double sP, int rem, bool isSpecial);
	~Book();

	double getPrice();
};

class Clothes : public Product {
public:
	Clothes(string name, string desc, double oP, double sP, int rem, bool isSpecial);
	~Clothes();

	double getPrice();
};

class Electronic : public Product {
public:
	Electronic(string name, string desc, double oP, double sP, int rem, bool isSpecial);
	~Electronic();
	double getPrice();
};

class Food : public Product {
public:
	Food(string name, string desc, double oP, double sP, int rem, bool isSpecial);
	~Food();
	double getPrice();
};