#include "pch.h"
#include "generator.h"

double Generator::uniform(int max_, int min_) //generacja rozk³adu równomiernego
{
	unsigned long int h_ = x_ / q_;
	x_ = (a_ * (x_ - (q_ * h_))) - (r_ * h_);
	if (x_ < 0) x_ += rmax_;
	return ((max_ - min_)*x_ + min_) / rmax_;
}

double Generator::normal(double mean_, double std_) //generacja rozk³adu normalnego
{
	double y_ = 0;
	for (int i_ = 0; i_ < 12; i_++) y_ += uniform(1, 0);
	y_ -= 6;
	return  y_ * std_ + mean_;
}

double Generator::expotential(double lambda_) //generacja rozk³adu wyk³adniczego
{
	return (-lambda_)*log(1 - uniform(1, 0));
}
Generator::Generator(int a)
{
	x_ = seed_[a];
}


Generator::~Generator()
{
}
