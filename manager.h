#pragma once
#include "clients.h"
class Manager//klasa implementująca kierownika sali
{
public:
	bool manag_flag_; //flaga dostępności kierownika
    double time_=55;  // czas obsługi kierownika
	Clients *c_;
	Manager();
	~Manager();
};

