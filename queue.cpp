#include "pch.h"
#include "queue.h"

void Queue::remove_deque()
{
	delete_all(client_);
	client_.clear();
	delete_all(client_buff_);
	client_buff_.clear();
	delete_all(client_cash_);
	client_cash_.clear();
	delete_all(client_drink_);
	client_drink_.clear();
	delete_all(client_in_buffet_);
	client_in_buffet_.clear();
	delete_all(client_meal_);
	client_meal_.clear();
	delete_all(client_tab_);
	client_tab_.clear();
	
}

void Queue::delete_all(std::deque<Clients*>& cl_)
{
	for (int i_ = 0; i_ < cl_.size(); i_++)
	{
		delete cl_[i_]->w_;
		delete cl_[i_];
	}
}

Queue::Queue()
{
	cl_buff_iter_ = 0;
	cash_iter_ = 0;
	cl_iter_ = 0;
	cl_que_iter_ = 0;
	meas_iter_ = 0;
	time_all_rest_ = 0;
	time_rest_que_ = 0;
	rest_que_size_ = 0;
}


Queue::~Queue()
{
}
