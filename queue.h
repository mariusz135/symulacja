#pragma once
#include "clients.h"
#include <deque>
class Queue//klasa implementuj¹ca kolejkê klientów
{
public:
	std::deque<Clients*>client_; //lista klientów
	std::deque<Clients*>client_cash_; //kolejka do kasjerów
	std::deque<Clients*>client_buff_; //kolejka do bufetu
	std::deque<Clients*>client_tab_; //klienci na sali
	std::deque<Clients*>client_in_buffet_; //klienci w bufecie
	std::deque<Clients*>client_drink_; //klienci oczekuj¹cy na napoje
	std::deque<Clients*>client_meal_; //klienci oczekuj¹cy na danie
	unsigned int cl_iter_; //iteratory klientów, klientów do kasjerów, klientów w bufecie, klientów w g³ownej kolejce
	unsigned int cash_iter_;
	unsigned int cl_buff_iter_;
	unsigned int cl_que_iter_;
	unsigned int meas_iter_;
	double time_all_rest_;
	double time_rest_que_;
	double rest_que_size_;
	double time_all_cash_;
	double time_cash_que_;
	double cash_que_size_;
	void remove_deque();
	void delete_all(std::deque<Clients*>& cl_);
	Queue();
	~Queue();
};

