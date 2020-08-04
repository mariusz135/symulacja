#pragma once
#include <string>
#include "restaurant.h"
#include <fstream>
#include "generator.h"
class Process;

class Event
{
public:
	double event_time_; //czas zdarzenia
	bool event_trig_; //flaga zdarzenia
	std::string id_;
	void execute(std::list <Event*>& agenda_, Generator *gen_, Restaurant *rest_, std::string ch_, std::fstream &file_, int &id_,  double clock_, std::vector<double>&rest_que_, std::vector<double>&cash_que_,  std::fstream &file2_);
	void alarm(std::deque<Clients*> &q_, Generator* gen_, std::string ch_, std::fstream &file_, Restaurant *rest_, std::fstream &file2_);
	Event(double time_, std::string a_, double clock_);
	~Event();
	
};

