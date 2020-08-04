#pragma once
#include "waiters.h"
class Clients //klasa implementuj¹ca klientów
{
public:
    Waiters *w_;
	int id_;
	int number_; //licznoœc grupy klientów
	int num_tab2_;
	int num_tab3_;
	int num_tab4_;
    double time_table_;
	double time_waiter_;
	Clients(int a_, int b_);
	~Clients();
};

