#include "pch.h"
#include "clients.h"


Clients::Clients(int a, int b):id_(a), number_(b)
{
	num_tab2_ = 0;
	num_tab3_ = 0;
	num_tab4_ = 0;
	time_table_ = 0;
	time_waiter_ = 0;
	w_ = nullptr;
}


Clients::~Clients()
{
}
