#pragma once
#include "clients.h"
class Manager//klasa implementuj�ca kierownika sali
{
public:
	bool manag_flag_; //flaga dost�pno�ci kierownika
    double time_=55;  // czas obs�ugi kierownika
	Clients *c_;
	Manager();
	~Manager();
};

