#pragma once
#include "waiters.h"
class Clients //klasa implementuj�ca klient�w
{
public:
    Waiters *w_;
	int id_;
	int number_; //liczno�c grupy klient�w
	int num_tab2_;
	int num_tab3_;
	int num_tab4_;
    double time_table_;
	double time_waiter_;
	Clients(int a_, int b_);
	~Clients();
};

