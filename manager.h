#pragma once
#include "clients.h"
class Manager//klasa implementuj¹ca kierownika sali
{
public:
	bool manag_flag_; //flaga dostêpnoœci kierownika
    double time_=55;  // czas obs³ugi kierownika
	Clients *c_;
	Manager();
	~Manager();
};

