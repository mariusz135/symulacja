#pragma once
#include <cmath>
class Generator
{
public:
	long int rmax_ = 2147483647;
	long int a_ = 69621;
	long int q_ = 30485;
	long int r_ = 23902;
	double x_;
	long int seed_[20]={ 1920099454, 342908074, 1675052770, 1915694207, 1769893299, 758294348, 1941466492, 563152444, 972483375, 926493346, 767785459, 582282466, 786177889, 2135081774, 1263353817, 1683009695, 387596776, 1924320932, 1964288986, 1122463498};
	double uniform(int max_, int min_);
	double normal(double mean_, double std_);
	double expotential(double lambda_);
	Generator(int a_);
	~Generator();
};

