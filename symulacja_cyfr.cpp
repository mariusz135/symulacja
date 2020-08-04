// symulacja_cyfr.cpp : Ten plik zawiera funkcję „main”. W nim rozpoczyna się i kończy wykonywanie programu.
//

#include "pch.h"
#include "restaurant.h" 
#include "cashiers.h"
#include "clients.h"
#include "queue.h"
#include "tables.h"
#include "waiters.h"
#include "manager.h"
#include "generator.h"
#include "event.h"
#include <string>
#include <iostream>
#include <fstream>
#include <cmath>
#include "symulacja_cyfr.h"
using namespace std;

std::list<Event*>::iterator find_minimum(std::list<Event*>& agenda_) //znajdowanie chronologicznie pierwszego zdarzenia-o najmniejszym czasie, najbliższym
{
	
	std::list<Event*>::iterator i_;
	i_ = agenda_.begin();
	std::list<Event*>::iterator pom_=i_;
	std::list<Event*>::iterator pom2_;
	advance(i_, 1);
	for ( i_; i_!=agenda_.end(); i_++) 
	{
		pom2_ = i_;
		if ((*pom_)->event_time_ > (*pom2_)->event_time_) pom_ = pom2_;
	}
	return pom_;
}
void del(std::list<Event*> & agenda_, Event *x_) //usuwanie wybranego zdarzenia z listy
{
	Event* pom2_;
	std::list<Event*>::iterator i_;
	for ( i_=agenda_.begin(); i_!=agenda_.end(); i_++) 
	{
		pom2_ = *i_;
		if (pom2_ == x_) 
		{
			agenda_.erase(i_);
			break;
		}
	}
	
}
static bool delete_all_events(Event* element_)
{
	delete element_;
	return true;
}
static bool delete_all_tables(Tables* element_)
{
	delete element_;
	return true;
}
static bool delete_all_waiters(Waiters* element_)
{
	delete element_;
	return true;
}
static bool delete_all_cashiers(Cashiers* element_)
{
	delete element_->cl_;
	delete element_;
	return true;
}
double mean(int n_, double T_[])//średnia
{
	double u_ = 0;
	for (int i_ = 0; i_ < n_; i_++) u_ += T_[i_];
	return u_ /= n_;
}
double var(int n_, double T_[])// estymator wariancji
{
	double u_ = 0;
	u_ = mean(n_, T_);
	double u1_ = 0;
	for (int i_ = 0; i_ < n_; i_++) u1_ += pow(T_[i_],2);
	u1_ /= n_-1;
	return u1_ - (((n_*pow(u_, 2))) / (n_ - 1));
}

int main()
{   
   
	int num_simul_;//liczba symulacjii
	string access_; //zmienna do ustawiania trybu krokowego lub ciągłego
	string x_;
	double t_st_[30] = {12.706, 4.303, 3.182, 2.776, 2.571, 2.447,2.365, 2.306, 2.262, 2.228, 2.201, 2.179, 2.16, 2.145, 2.131, 2.12, 2.11, 2.101, 2.093, 2.086, 2.08, 2.074, 2.069, 2.064, 2.06, 2.056, 2.052, 2.048, 2.045, 2.042}; //tablica z wartościami t Studenta dlaa 95% przedziału ufności
	double simul_time_;//czas trwania symulacji
	cout << "Set number of simulations ";
	cin >> num_simul_;
    cout << endl;
	cout << "Set simulation time "; //ustawienie czasu symulacji
	cin >> simul_time_;
	cout << endl;
	cout << "Step or continous? s/c "; //wybór tryb krokowy/ ciągły
	cin >> access_;
	cout << endl;
	double *restque_ = new double[num_simul_];
	double *cashque_ = new double[num_simul_];
	double *timetable_ = new double[num_simul_];
	double *timewaiter_ = new double[num_simul_];
	fstream file_, file2_; // zmienne plikowe
	file_.open("symulacja.txt", std::ios::out);
	for(int i_=0; i_<num_simul_; i_++)
	{
		int id_ = 0;
		vector<double>rest_que_; //wektory do zbierania danych do statystyki
		vector<double>cash_que_;
		vector<double>time_for_table_; 
		vector<double>time_for_waiter_;
		string name_;//zmienna do nazywania plików
		double clock_ = 0; //zegar
		Restaurant *rest_ = new Restaurant(); //wskaźnik do obiektów restauracji
        Generator *gen_ = new Generator(i_);
		std::list <Event*> agenda_;// lista zdarzeń
		
		for (int i_ = 0; i_ < rest_->table_2_; i_++) rest_->table2_.push_back(new Tables(2)); //listy wszystkich stolików
		for (int i_ = 0; i_ < rest_->table_3_; i_++) rest_->table3_.push_back(new Tables(3));
		for (int i_ = 0; i_ < rest_->table_4_; i_++) rest_->table4_.push_back(new Tables(4));
		for (int i_ = 0; i_ < rest_->waiters_num_; i_++) rest_->waiter_.push_back(new Waiters()); //lista wszystkich klenerów
		for (int i_ = 0; i_ < rest_->cashier_num_; i_++) rest_->cashier_.push_back(new Cashiers()); // lista wszystkich kasjerów
		//zapis do plików
		name_ = "klient";
		name_ += to_string(i_);
		name_ += ".txt";
		file2_.open(name_, std::ios::out);
		if (access_ == "s") cout << "Simulation nr " << i_ << endl;
		file_ << "Simulation nr " << i_ << endl;
		agenda_.push_back(new Event(gen_->normal(rest_->u_a_, rest_->b_a_), "ClientsArrived",clock_));//zapalnowanie pojawienia się pierwwzej grupy klientów i alarmu
		agenda_.push_back(new Event(gen_->normal(rest_->u_e_, rest_->b_e_), "Alarm", clock_));
		while (clock_ < simul_time_) //pętla symulacyjna
		{
			if (!agenda_.empty())
			{
				std::list<Event*>::iterator it_ = find_minimum(agenda_); //znalezienie pierwszego chronologicznie zdarzenia
				clock_ = (*it_)->event_time_;//inkrementacja czasu
				(*it_)->execute(agenda_,gen_, rest_, access_, file_, id_,  clock_, rest_que_, cash_que_, file2_);
				while ((*it_)->event_trig_)
				{
					(*it_)->event_trig_ = false;
					if (!rest_->que_.client_.empty()) //mendżer pobiera grupę z kolejki
					{
						if (rest_->manag_->manag_flag_) //czy jest wolny i czy obsłużył poprzednich klientów
						{
							if (!rest_->manag_->c_)
							{
								rest_->manag_->c_ = rest_->que_.client_.front(); //zabranie klientów z kolejki
								rest_->manag_->c_->time_table_ = clock_;
								rest_->que_.cl_que_iter_ -= rest_->que_.client_.front()->number_;
								if (rest_->que_.meas_iter_ >= 200)
								{
									rest_->que_.rest_que_size_ = rest_->que_.cl_que_iter_*(clock_ - rest_->que_.time_rest_que_);
									rest_->que_.time_all_rest_ += clock_ - rest_->que_.time_rest_que_;
									rest_->que_.time_rest_que_ = clock_;
									rest_que_.push_back(rest_->que_.rest_que_size_); //jeśli można zbierać statystyki
								}
								rest_->que_.client_.pop_front();
								rest_->manag_->manag_flag_ = false; //menadżer zajęty
								if (access_ == "s") cout << "Manager takes clients from front of queue" << endl;
								file_ << "Manager takes clients from front of queue" << endl;
								agenda_.push_back(new Event(rest_->manag_->time_, "Manager", clock_)); //zaplanowanie komunikatau menadżera
                                (*it_)->event_trig_ = true;
							}
						}
						
					}
					if (!rest_->manag_->manag_flag_ && rest_->manag_->c_)  //przydział stolika
					{
						if (access_ == "s") //jeśli tryb krokowy
						{
							cout << "Press any key to continue" << endl; //zapis na potrzeby pracy krokowej
							cin >> x_;
							cout << "Clients go with manager to tables" <<endl;

						}
						file_<< "Clients go with manager to tables" <<endl;
						switch (rest_->manag_->c_->number_)
						{
						  case 1: //przychodzi grupa jednosobowa
							  if (!rest_->table2_.empty())
							  {
								  rest_->manag_->c_->num_tab2_ = 1; //ilość zajętych stolików przez kelenera
								  rest_->manag_->c_->time_waiter_ = clock_; //czekanie na kelnera
								  rest_->manag_->c_->time_table_ = clock_ - rest_->manag_->c_->time_table_; //ustalanie czasu zajęcia stolika
								  if (rest_->que_.meas_iter_ >= 200)
								  {
									  if (rest_->manag_->c_->time_table_ != 0) time_for_table_.push_back(rest_->manag_->c_->time_table_); //jeśli to zdarzenie wykonało się natychmiast po porzednim to menadżer wykonał swoje zadanie ze swoim minilanym czasem. Poostaje czekać tylko na komunikat
									  else  time_for_table_.push_back(rest_->manag_->time_);
								  }
								  rest_->que_.client_tab_.push_back(rest_->manag_->c_);
								  if (access_ == "s") cout << "Clients have a 2-seater table" << endl;
								  file_ << "Clients have a 2-seater table" << endl;
								  rest_->table2_.pop_front();
                                  rest_->manag_->c_ = nullptr; //menadżer wolny 
								  rest_->manag_->manag_flag_ = true;
								 (* it_)->event_trig_ = true; //flaga zdarzenia warunkowego
							  }
							  else if (!rest_->table3_.empty())
							  {
								  rest_->manag_->c_->num_tab3_ = 1;
								  rest_->manag_->c_->time_waiter_ = clock_;
								  rest_->manag_->c_->time_table_ = clock_ - rest_->manag_->c_->time_table_;
								  if (rest_->que_.meas_iter_ >= 200)
								  {
									  if (rest_->manag_->c_->time_table_ != 0) time_for_table_.push_back(rest_->manag_->c_->time_table_);
									 else time_for_table_.push_back(rest_->manag_->time_);
								  }
								  rest_->que_.client_tab_.push_back(rest_->manag_->c_);
								  if (access_ == "s") cout << "Clients have a 3-seater table" << endl;
								  file_ << "Clients have a 3-seater table" << endl;
								  rest_->table3_.pop_front();
								  rest_->manag_->c_ = nullptr;
								  rest_->manag_->manag_flag_ = true;
								 (*it_)->event_trig_ = true;
							  }
							  else if (!rest_->table4_.empty())
							  {
								  rest_->manag_->c_->num_tab4_ = 1;
								  rest_->manag_->c_->time_waiter_ = clock_;
								  rest_->manag_->c_->time_table_ = clock_ - rest_->manag_->c_->time_table_;
								  if (rest_->que_.meas_iter_ >= 200)
								  {
									  if (rest_->manag_->c_->time_table_ != 0) time_for_table_.push_back(rest_->manag_->c_->time_table_);
									  else time_for_table_.push_back(rest_->manag_->time_);
								  }
								  rest_->que_.client_tab_.push_back(rest_->manag_->c_);
								  if (access_ == "s") cout << "Clients have a 4-seater table" << endl;
								  file_ << "Clients have a 4-seater table" << endl;
								  rest_->table4_.pop_front();
								  rest_->manag_->c_ = nullptr;
								  rest_->manag_->manag_flag_ = true;
								  (*it_)->event_trig_ = true;
							  }
							 
							 break;
						  case 2: //przychodiz grupa dwuosobowa
							  if (!rest_->table2_.empty())
							  {
								  rest_->manag_->c_->num_tab2_ = 1;
								  rest_->manag_->c_->time_waiter_ = clock_;
								  rest_->manag_->c_->time_table_ = clock_ - rest_->manag_->c_->time_table_;
								  if (rest_->que_.meas_iter_ >= 200)
								  {
									  if (rest_->manag_->c_->time_table_ != 0) time_for_table_.push_back(rest_->manag_->c_->time_table_);
									  else time_for_table_.push_back(rest_->manag_->time_);
								  }
								  rest_->que_.client_tab_.push_back(rest_->manag_->c_);
								  if (access_ == "s") cout << "Clients have a 2-seater table" << endl;
								  file_ << "Clients have a 2-seater table" << endl;
								  rest_->table2_.pop_front();
								  rest_->manag_->c_ = nullptr;
								  rest_->manag_->manag_flag_ = true;
								  (*it_)->event_trig_ = true;
							  }
							  else if (!rest_->table3_.empty())
							  {
								  rest_->manag_->c_->num_tab3_ = 1;
								  rest_->manag_->c_->time_waiter_ = clock_;
								  rest_->manag_->c_->time_table_ = clock_ - rest_->manag_->c_->time_table_;
								  if (rest_->que_.meas_iter_ >= 200)
								  {
									  if (rest_->manag_->c_->time_table_ != 0) time_for_table_.push_back(rest_->manag_->c_->time_table_);
									  else time_for_table_.push_back(rest_->manag_->time_);
								  }
								  rest_->que_.client_tab_.push_back(rest_->manag_->c_);
								  if (access_ == "s") cout << "Clients have a 3-seater table" << endl;
								  file_ << "Clients have a 3-seater table" << endl;
								  rest_->table3_.pop_front();
								  rest_->manag_->c_ = nullptr;
								  rest_->manag_->manag_flag_ = true;
								  (*it_)->event_trig_ = true;
							  }
							  else if (!rest_->table4_.empty())
							  {
								  rest_->manag_->c_->num_tab4_ = 1;
								  rest_->manag_->c_->time_waiter_ = clock_;
								  rest_->manag_->c_->time_table_ = clock_ - rest_->manag_->c_->time_table_;
								  if (rest_->que_.meas_iter_ >= 200)
								  {
									 if (rest_->manag_->c_->time_table_ != 0) time_for_table_.push_back(rest_->manag_->c_->time_table_);
									  else  time_for_table_.push_back(rest_->manag_->time_);
								  }
								  rest_->que_.client_tab_.push_back(rest_->manag_->c_);
								  if (access_ == "s") cout << "Clients have a 4-seater table" << endl;
								  file_ << "Clients have a 4-seater table" << endl;
								  rest_->table4_.pop_front();
								  rest_->manag_->c_ = nullptr;
								  rest_->manag_->manag_flag_ = true;
								  (*it_)->event_trig_ = true;
							  }
							  break;
						  case 3: //przychodzi grupa trzyosobowa
							  if (!rest_->table3_.empty())
							  {
								  rest_->manag_->c_->num_tab3_ = 1;
								  rest_->manag_->c_->time_waiter_ = clock_;
								  rest_->manag_->c_->time_table_ = clock_ - rest_->manag_->c_->time_table_;
								  if (rest_->que_.meas_iter_ >= 200)
								  {
									  if (rest_->manag_->c_->time_table_ != 0) time_for_table_.push_back(rest_->manag_->c_->time_table_);
									  else time_for_table_.push_back(rest_->manag_->time_);
								  }
								  rest_->que_.client_tab_.push_back(rest_->manag_->c_);
								  if (access_ == "s") cout << "Clients have a 3-seater table" << endl;
								  file_ << "Clients have a 3-seater table" << endl;
								  rest_->table3_.pop_front();
								  rest_->manag_->c_ = nullptr;
								  rest_->manag_->manag_flag_ = true;
								  (*it_)->event_trig_ = true;
							  }
							  else if (rest_->table2_.size() >= 2) //jesli są 2 stoliki dwuosobowe
							  {
								 
								  rest_->table2_.pop_front();
								  rest_->table2_.pop_front();
								  rest_->manag_->c_->num_tab2_ = 2;
								  rest_->manag_->c_->time_waiter_ = clock_;
								  rest_->manag_->c_->time_table_ = clock_ - rest_->manag_->c_->time_table_;
								  if (rest_->que_.meas_iter_ >= 200)
								  {
									  if (rest_->manag_->c_->time_table_ != 0) time_for_table_.push_back(rest_->manag_->c_->time_table_);
									  else time_for_table_.push_back(rest_->manag_->time_);
								  }
								  rest_->que_.client_tab_.push_back(rest_->manag_->c_);
								  if (access_ == "s") cout << "Clients have a 2 2-seater tables" << endl;
								  file_ << "Clients have a 2 2-seater tables" << endl;
								  rest_->manag_->c_ = nullptr;
								  rest_->manag_->manag_flag_ = true;
								  (*it_)->event_trig_ = true;
							  }
							  else if (!rest_->table4_.empty())
							  {
								  rest_->manag_->c_->num_tab4_ = 1;
								  rest_->manag_->c_->time_waiter_ = clock_;
								  rest_->manag_->c_->time_table_ = clock_ - rest_->manag_->c_->time_table_;
								  if (rest_->que_.meas_iter_ >= 70)
								  {
									  if (rest_->manag_->c_->time_table_ != 0) time_for_table_.push_back(rest_->manag_->c_->time_table_);
									  else  time_for_table_.push_back(rest_->manag_->time_);
								  }
								  rest_->que_.client_tab_.push_back(rest_->manag_->c_);
								  if (access_ == "s") cout << "Clients have a 4-seater table" << endl;
								  file_ << "Clients have a 4-seater table" << endl;
								  rest_->table4_.pop_front();
								  rest_->manag_->c_ = nullptr;
								  rest_->manag_->manag_flag_ = true;
								  (*it_)->event_trig_ = true;
							  }
							  break;
						  case 4: //przychodiz grupa czterosobowa
							  if (!rest_->table4_.empty())
							  {
								  rest_->manag_->c_->num_tab4_ = 1;
								  rest_->manag_->c_->time_waiter_ = clock_;
								  rest_->manag_->c_->time_table_ = clock_ - rest_->manag_->c_->time_table_;
								  if (rest_->que_.meas_iter_ >= 200)
								  {
									 if (rest_->manag_->c_->time_table_ != 0) time_for_table_.push_back(rest_->manag_->c_->time_table_);
									  else time_for_table_.push_back(rest_->manag_->time_);
								  }
								  rest_->que_.client_tab_.push_back(rest_->manag_->c_);
								  if (access_ == "s") cout << "Clients have a 4-seater table" << endl;
								  file_ << "Clients have a 4-seater table" << endl;
								  rest_->table4_.pop_front();
								  rest_->manag_->c_ = nullptr;
								  rest_->manag_->manag_flag_ = true;
								  (*it_)->event_trig_ = true;
							  }
							  else if (rest_->table2_.size()>=2) //jesli są 2 stoliki dwuosobowe
							  {
								  rest_->table2_.pop_front();
								  rest_->table2_.pop_front();
								  rest_->manag_->c_->num_tab2_ = 2;
								  rest_->manag_->c_->time_waiter_ = clock_;
								  rest_->manag_->c_->time_table_ = clock_ - rest_->manag_->c_->time_table_;
								  if (rest_->que_.meas_iter_ >= 200)
								  {
									  if (rest_->manag_->c_->time_table_ != 0) time_for_table_.push_back(rest_->manag_->c_->time_table_);
									  else time_for_table_.push_back(rest_->manag_->time_);
								  }
								  rest_->que_.client_tab_.push_back(rest_->manag_->c_);
								  if (access_ == "s") cout << "Clients have a 2 2-seater tables" << endl;
								  file_ << "Clients have a 2 2-seater tables" << endl;
								  rest_->manag_->c_ = nullptr;
								  rest_->manag_->manag_flag_ = true;
								  (*it_)->event_trig_ = true;
							  }
							  else if (!rest_->table3_.empty() && !rest_->table2_.empty()) //jęsli jest stolik dwuosobowy i trzy osobowy
							  {
								  rest_->table2_.pop_front();
								  rest_->table3_.pop_front();
								  rest_->manag_->c_->num_tab2_ = 1;
								  rest_->manag_->c_->num_tab3_ = 1;
								  rest_->manag_->c_->time_waiter_ = clock_;
								  rest_->manag_->c_->time_table_ = clock_ - rest_->manag_->c_->time_table_;
								  if (rest_->que_.meas_iter_ >= 200)
								  {
									  if (rest_->manag_->c_->time_table_ != 0) time_for_table_.push_back(rest_->manag_->c_->time_table_);
									  else time_for_table_.push_back(rest_->manag_->time_);
								  }
								  rest_->que_.client_tab_.push_back(rest_->manag_->c_);
								  if (access_ == "s") cout << "Clients have a 2-seater and 3-seater tables" << endl;
								  file_ << "Clients have a 2-seater  and 3-seater tables" << endl;
								  rest_->manag_->c_ = nullptr;
								  rest_->manag_->manag_flag_ = true;
								  (*it_)->event_trig_ = true;
							  }
							  else if (rest_->table3_.size()>=2) //jeśli są 2 stoliki trzyosobowe
							  {
								  rest_->table3_.pop_front();
								  rest_->table3_.pop_front();
								  rest_->manag_->c_->num_tab3_ = 2;
								  rest_->manag_->c_->time_waiter_ = clock_;
								  rest_->manag_->c_->time_table_ = clock_ - rest_->manag_->c_->time_table_;
								  if (rest_->que_.meas_iter_ >= 200)
								  {
									  if (rest_->manag_->c_->time_table_ != 0) time_for_table_.push_back(rest_->manag_->c_->time_table_);
									  else time_for_table_.push_back(rest_->manag_->time_);
								  }
								  rest_->que_.client_tab_.push_back(rest_->manag_->c_);
								  if (access_ == "s") cout << "Clients have a 2 3-seater tables" << endl;
								  file_ << "Clients have a 2 3-seater tables" << endl;
								  rest_->manag_->c_ = nullptr;
								  rest_->manag_->manag_flag_ = true;
								  (*it_)->event_trig_ = true;
							  }
							  break;
							  
						}
						
						
					}
					
					if (!rest_->waiter_.empty() && !rest_->que_.client_tab_.empty()) //jeśli są wolni kelnerzy i klineci na sali
					{
						rest_->que_.client_tab_.front()->w_ = rest_->waiter_.front(); //przydział kelnera do stolika
						rest_->que_.client_tab_.front()->time_waiter_ = clock_ - rest_->que_.client_tab_.front()->time_waiter_;
						if (rest_->que_.meas_iter_ >= 200) time_for_waiter_.push_back(rest_->que_.client_tab_.front()->time_waiter_); //zbieranie satystyk
						rest_->waiter_.pop_front();
						if (access_ == "s") //jeśli tryb krokowy
						{
							cout << "Press any key to continue" << endl; //zapis na potrzeby pracy krokowej
							cin >> x_;
							cout << "Waiter accepts order. Clients with id " << rest_->que_.client_tab_.front()->id_ <<" will get a drink"<< endl;

						}
						file_ << "Waiter accepts order. Clients with id " << rest_->que_.client_tab_.front()->id_ << " will get a drink" << endl;
						rest_->que_.client_drink_.push_back(rest_->que_.client_tab_.front()); //klienci czekają na napoje
						rest_->que_.client_tab_.pop_front(); 
						agenda_.push_back(new Event(gen_->expotential(rest_->lambda_n_), "Drinks",clock_));
						(*it_)->event_trig_ = true; 
					}
					if (!rest_->que_.client_buff_.empty()) //jesli są miejsca w bufecie
					{
					if (rest_->places_in_bufet_ > (rest_->que_.cl_buff_iter_ + rest_->que_.client_buff_.front()->number_))
					{
						if (access_ == "s")
						{
							std::cout << "Press any key to continue" << std::endl;
							std::cin >> x_;
							std::cout << "Clients in buffet que" << std::endl;
						}
						file_ << "Clients in buffet que" << std::endl;
						rest_->que_.cl_buff_iter_ += rest_->que_.client_buff_.front()->number_; //inkrementacja iteratora
						if (access_ == "s") std::cout << "Clients with id " << rest_->que_.client_buff_.front()->id_ << " in buffet" << std::endl;
						file_ << "Clients with id " << rest_->que_.client_buff_.front()->id_ << " in buffet" << std::endl;
						rest_->que_.client_in_buffet_.push_back(rest_->que_.client_buff_.front()); //ludzie opuszczaja kolejkę do bufetu i zajmują miejsca w nim
						rest_->que_.client_buff_.pop_front();
						agenda_.push_back(new Event(gen_->normal(rest_->u_b_, rest_->b_b_), "Buffet", clock_)); //zaplanowanie końca konsumpcji w bufecie
						(*it_)->event_trig_ = true;
					}
					}
					//ludzie w kolejce do kasjerów
					if (!rest_->cashier_.empty())
					{
						if (!rest_->que_.client_cash_.empty())
						{
							if (access_ == "s")
							{
								std::cout << "Press any key to continue" << std::endl;
								std::cin >> x_;
								std::cout << "Clients go to cashiers" << std::endl;
							}
							file_ << "Clients go to cashiers " << std::endl;
							for (std::list<Cashiers*>::iterator it = rest_->cashier_.begin(); it != rest_->cashier_.end(); it++)
							{
								if ((*it)->cl_ == nullptr) //czy jest wolnu jakiś kasjer
								{
									(*it)->cl_=rest_->que_.client_cash_.front(); //zabranie klientow z kolejki
									rest_->que_.cash_iter_ -= rest_->que_.client_cash_.front()->number_;
									if (rest_->que_.meas_iter_ >= 200)
									{
										rest_->que_.cash_que_size_ = rest_->que_.cash_iter_*(clock_ - rest_->que_.time_cash_que_);
										rest_->que_.time_all_cash_ += clock_ - rest_->que_.time_cash_que_;
										rest_->que_.time_cash_que_ = clock_;
										cash_que_.push_back(rest_->que_.cash_que_size_);
									}
                                    rest_->que_.client_cash_.pop_front();
									break;
								}
							}
							agenda_.push_back(new Event(gen_->expotential(rest_->lambda_p_), "End", clock_)); //zaplanowanie końca obsługi
							(*it_)->event_trig_ = true;
						}
					} 

				}
				delete (*it_);
				agenda_.erase(it_);
				//usunięcie zdarzenia z listy

			}
			else break;

		}
		//zamknięcie plików
		
		file2_.close();
		int sum_ = 0;
		for (int i_ = 0; i_ < rest_que_.size(); i_++) sum_ += rest_que_[i_];
		restque_[i_] = (double)sum_ /rest_->que_.time_all_rest_;
		sum_ = 0;
		for (int i_ = 0; i_ < cash_que_.size(); i_++) sum_ += cash_que_[i_]; //wyznaczenie średnich
		cashque_[i_] = (double)sum_ / rest_->que_.time_all_cash_;
		sum_ = 0;
		for (int i_ = 0; i_ < time_for_table_.size(); i_++) sum_ += time_for_table_[i_];
		timetable_[i_] = (double)sum_ / time_for_table_.size();
		sum_ = 0;
		for (int i_ = 0; i_ < time_for_waiter_.size(); i_++) sum_ += time_for_waiter_[i_];
		timewaiter_[i_] = (double)sum_ / time_for_waiter_.size();
		delete gen_;
		rest_->que_.remove_deque();
		delete rest_->manag_->c_;
		delete rest_->manag_;
		rest_->waiter_.remove_if(delete_all_waiters);
		rest_->waiter_.clear();
		rest_->cashier_.remove_if(delete_all_cashiers);
		rest_->cashier_.clear();
		rest_->table2_.remove_if(delete_all_tables);
		rest_->table2_.clear();
		rest_->table3_.remove_if(delete_all_tables);
		rest_->table3_.clear();
		rest_->table4_.remove_if(delete_all_tables);
		rest_->table4_.clear();
		delete rest_; //czyszczenie na nastepną symulację
		agenda_.remove_if(delete_all_events);
		agenda_.clear();
		rest_que_.clear();
		cash_que_.clear();
		time_for_table_.clear();
		time_for_waiter_.clear();
	}
	file_.close();
	//test generatorów pseudolosowych 
   /* Generator *gen = new Generator(0);
	fstream roz;
	roz.open("wykladniczy.txt", ios::out);
	for (int i = 0; i <= 10000; i++) roz <<gen->expotential(1500)<< endl;
	roz.close();
	delete gen;
	gen = new Generator(1);
	fstream roz2;
	roz2.open("normalny.txt", ios::out);
	for (int i = 0; i <= 10000; i++) roz2 <<gen->normal(1500,50) << endl;
	roz2.close();
	delete gen;
	gen = new Generator(2);
	fstream roz3;
	roz3.open("rownomierny.txt", ios::out);
	for (int i = 0; i <= 10000; i++) roz3 <<gen->uniform(10000,0) << endl;
	roz3.close();
	delete gen;
		//podanie wyników i przedziałów ufności*/
	cout << "a) Average waiting time for table " << mean(num_simul_, timetable_) << " Convidence interval from " << mean(num_simul_, timetable_) - (t_st_[num_simul_ - 2] * (sqrt(var(num_simul_, timetable_) / num_simul_))) << " to " << mean(num_simul_, timetable_) + (t_st_[num_simul_ - 2] * (sqrt(var(num_simul_, timetable_) / num_simul_)))<< endl;
	cout << "b) Average lenght of the queue to tables "<<mean(num_simul_,restque_)<<" Convidence interval from "<<mean(num_simul_,restque_)-(t_st_[num_simul_-2]*(sqrt(var(num_simul_,restque_)/num_simul_)))<< " to " << mean(num_simul_, restque_) + (t_st_[num_simul_-2] * (sqrt(var(num_simul_, restque_) / num_simul_))) << endl;
	cout << "c) Average waiting time for waiter " << mean(num_simul_, timewaiter_) <<" Convidence interval from " << mean(num_simul_, timewaiter_) - (t_st_[num_simul_ - 2] * (sqrt(var(num_simul_, timewaiter_) / num_simul_))) << " to " << mean(num_simul_, timewaiter_) + (t_st_[num_simul_ - 2] * (sqrt(var(num_simul_, timewaiter_) / num_simul_)))<< endl;
	cout << "d) Average length of the queue to cashiers " << mean(num_simul_, cashque_) <<" Convidence interval from " << mean(num_simul_, cashque_) - (t_st_[num_simul_ - 2] * (sqrt(var(num_simul_, cashque_) / num_simul_))) << " to " << mean(num_simul_, cashque_) + (t_st_[num_simul_ - 2] * (sqrt(var(num_simul_, cashque_) / num_simul_)))<< endl;
    std::cout << "Hello World!\n"; 
	
}

// Uruchomienie programu: Ctrl + F5 lub menu Debugowanie > Uruchom bez debugowania
// Debugowanie programu: F5 lub menu Debugowanie > Rozpocznij debugowanie

// Porady dotyczące rozpoczynania pracy:
//   1. Użyj okna Eksploratora rozwiązań, aby dodać pliki i zarządzać nimi
//   2. Użyj okna programu Team Explorer, aby nawiązać połączenie z kontrolą źródła
//   3. Użyj okna Dane wyjściowe, aby sprawdzić dane wyjściowe kompilacji i inne komunikaty
//   4. Użyj okna Lista błędów, aby zobaczyć błędy
//   5. Wybierz pozycję Projekt > Dodaj nowy element, aby utworzyć nowe pliki kodu, lub wybierz pozycję Projekt > Dodaj istniejący element, aby dodać istniejące pliku kodu do projektu
//   6. Aby w przyszłości ponownie otworzyć ten projekt, przejdź do pozycji Plik > Otwórz > Projekt i wybierz plik sln
