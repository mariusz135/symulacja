#include "pch.h"
#include "event.h"
#include <iostream>
#include <cmath>


void Event::execute(std::list <Event*>& agenda_,Generator *gen_, Restaurant *rest_, std::string ch_, std::fstream &file_, int &y_, double clock_, std::vector<double>&rest_que_, std::vector<double>&cash_que_,  std::fstream &file2_)
{
	std::string x_;
	if (id_ == "ClientsArrived")
	{
		if (ch_ == "s") //jeœli tryb krokowy
		{
			std::cout << "Press any key to continue" << std::endl; //zapis na potrzeby pracy krokowej
			std::cin >> x_;
			std::cout << "Clients arrived in restaurant" << std::endl;

		}
		Clients *c1_=nullptr;
		agenda_.push_back(new Event(gen_->normal(rest_->u_a_, rest_->b_a_), "ClientsArrived", clock_)); //planowanie nastepnej grupy
		// losowanie licznoœci klientów
		int q_ = gen_->uniform(100, 0);
		if (q_ >= 0 && q_ <= rest_->p1_ * 100) c1_ = new Clients(y_,1);
		else if (q_ > rest_->p1_ * 100 && q_ <= (rest_->p1_ * 100 + rest_->p2_ * 100)) c1_ = new Clients(y_, 2);
		else if (q_ > (rest_->p1_ * 100 + rest_->p2_ * 100) && q_ <= (rest_->p1_ * 10 + rest_->p2_ * 100 + rest_->p3_ * 100)) c1_ = new Clients(y_,3);
		else if (q_ > (rest_->p1_ * 100 + rest_->p2_ * 100 + rest_->p3_ * 100) && q_ <= (rest_->p1_ * 100 + rest_->p2_ * 100 + rest_->p3_ * 100 + rest_->p4_ * 100)) c1_ = new Clients(y_,4);
		y_++; //inkrementacja iteratora grup klientów
		if (c1_ != nullptr)
		{
			
			rest_->que_.meas_iter_++;
			rest_->que_.cl_iter_ += c1_->number_;
			file2_ << rest_->que_.cl_iter_ << std::endl;
		}
		if (q_< rest_->prop_buffet_ * 100) //jeœli do g³ownej kolejki
		{
			if (c1_ != nullptr)
			{
				
				rest_->que_.client_.push_back(c1_);  //klient zostaje przypisany do kolejki restauracji
			rest_->que_.cl_que_iter_ += c1_->number_;//zaspis wielkoœci kolejki do statystyki
		  
			if (rest_->que_.meas_iter_>=200)//dane do statysyk
				{
				rest_->que_.rest_que_size_ = rest_->que_.cl_que_iter_*(clock_ - rest_->que_.time_rest_que_);
				rest_->que_.time_all_rest_ += clock_ - rest_->que_.time_rest_que_;
				rest_->que_.time_rest_que_ = clock_;
				   rest_que_.push_back(rest_->que_.rest_que_size_);
					//pom = clock;
				}
			      
				if (ch_ == "s")
				{
					std::cout << "New Clients into a restaurant que. Clients id " << c1_->id_ << " .Group has " << c1_->number_ << " person" << std::endl;
					
				}
				file_ << "New Clients into a restaurant que. Clients id " << c1_->id_ << " .Group has " << c1_->number_ << " person" << std::endl;
	
			}
		}
		else
		{
			if (c1_ != nullptr)
			{
				//w przeciwnym razie klienci id¹ do bufetu
				rest_->que_.client_buff_.push_back(c1_); //zapisz klienta do kolejki do bufetu
				//rest->que.cl_buff_iter += c1->number; //zwiêksz iterator bufetu
				if (ch_ == "s") std::cout << "New Clients with id " << c1_->id_ << " into a buffet que" << std::endl;
				file_ << "New Clients with id " << c1_->id_ << " into a buffet que" << std::endl;
			}
		}
	
		
	}

	if (id_ == "Manager") //komunikat zdarzenia menad¿era
	{
		if (ch_ == "s") //jeœli tryb krokowy
		{
			std::cout << "Press any key to continue" << std::endl; //zapis na potrzeby pracy krokowej
			std::cin >> x_;
			if(rest_->manag_->manag_flag_) std::cout << "Manager is free" << std::endl; //wolny
			else std::cout << "Manager still wait for tables for clients" << std::endl;// jeszcze nie wolny

		}
		if (!rest_->manag_->manag_flag_ && rest_->manag_->c_) //przydzia³ stolika
		{
			Clients *pom;
			if (ch_ == "s") //jeœli tryb krokowy
			{
				std::cout << "Press any key to continue" <<std::endl; //zapis na potrzeby pracy krokowej
				std::cin >> x_;
				std::cout << "Clients go with manager to tables" << std::endl;

			}
			file_ << "Clients go with manager to tables" <<std::endl;
		


		}
		if (rest_->manag_->manag_flag_) file_ << "Manager is free" << std::endl;
		else file_ << "Manager still wait for tables for clients" << std::endl;
	}

	if (id_ == "Alarm") //alarm
	{
		if (ch_ == "s") //jeœli tryb krokowy
		{
			std::cout << "Press any key to continue" << std::endl; //zapis na potrzeby pracy krokowej
			std::cin >> x_;
			std::cout << "False alarm! Some clients can left restaurant" << std::endl;

		}
		file_ << "False alarm! Some clients can left restaurant" << std::endl;
		if (!rest_->que_.client_.empty()) alarm(rest_->que_.client_, gen_, ch_, file_, rest_, file2_);
	    if (!rest_->que_.client_buff_.empty()) alarm(rest_->que_.client_buff_, gen_, ch_, file_, rest_, file2_);
		if (!rest_->que_.client_cash_.empty()) alarm(rest_->que_.client_cash_, gen_, ch_, file_, rest_, file2_);
		if (!rest_->que_.client_tab_.empty()) alarm(rest_->que_.client_tab_, gen_, ch_, file_, rest_, file2_);
		agenda_.push_back(new Event(gen_->normal(rest_->u_e_, rest_->b_e_), "Alarm", clock_));
	}
	if (id_ == "Drinks")// podanie napojów
	{
		if (!rest_->que_.client_drink_.empty())
		{
			file_ << "Clients with id " << rest_->que_.client_drink_.front()->id_ << "  get a drinks. Clients await for a meal" << std::endl;
			if (ch_ == "s") //jeœli tryb krokowy
			{
				std::cout << "Press any key to continue" << std::endl; //zapis na potrzeby pracy krokowej
				std::cin >> x_;
				std::cout << "Clients with id " << rest_->que_.client_drink_.front()->id_ << "  get a drinks. Clients await for a meal" << std::endl;

			}
			
				rest_->que_.client_meal_.push_back(rest_->que_.client_drink_.front()); //klienci oczekuj¹ teraz na posi³ek
				rest_->que_.client_drink_.pop_front();
				agenda_.push_back(new Event(gen_->expotential(rest_->lambda_j_), "Meal", clock_)); //zaplanowanie podania posi³ku
			
            
		}
	}


	if (id_ == "Meal") //podanie posi³ku
	{
		if (ch_ == "s") //jeœli tryb krokowy
		{
			std::cout << "Press any key to continue" << std::endl; //zapis na potrzeby pracy krokowej
			std::cin >> x_;
			std::cout << "Clients with id " << rest_->que_.client_meal_.front()->id_ << "  get a meal. Clients start a consumption" << std::endl;

		}
		file_ << "Clients with id " << rest_->que_.client_meal_.front()->id_ << "  get a meal. Clients start a consumption" << std::endl;
		rest_->waiter_.push_back(rest_->que_.client_meal_.front()->w_); //klineci mog¹ zaczynaæ konsumpcje
		rest_->que_.client_meal_.front()->w_ = nullptr;
		agenda_.push_back(new Event(gen_->expotential(rest_->lambda_f_), "Consumption", clock_)); //planowanie zakoñczenia konsumpcji
		
	}
	if (id_ == "Consumption") //koniec konsumpcji kilentów na sali
	{
		if (ch_ == "s") //jeœli tryb krokowy
		{
			std::cout << "Press any key to continue" << std::endl; //zapis na potrzeby pracy krokowej
			std::cin >> x_;
			std::cout << "Clients with id " << rest_->que_.client_meal_.front()->id_ << "  end a consumption" << std::endl;

		}
		file_ << "Clients with id " << rest_->que_.client_meal_.front()->id_ << "  end a consumption" << std::endl;
		if (rest_->que_.client_meal_.front()->num_tab2_ != 0) for (int j = 0; j < rest_->que_.client_meal_.front()->num_tab2_; j++) rest_->table2_.push_back(new Tables(2)); //zwolnienie stolików
		if (rest_->que_.client_meal_.front()->num_tab3_ != 0) for (int j = 0; j < rest_->que_.client_meal_.front()->num_tab3_; j++) rest_->table2_.push_back(new Tables(3));
		if (rest_->que_.client_meal_.front()->num_tab4_ != 0) for (int j = 0; j < rest_->que_.client_meal_.front()->num_tab4_; j++) rest_->table2_.push_back(new Tables(4));
		rest_->que_.client_cash_.push_back(rest_->que_.client_meal_.front()); //klienci id¹ do kasjerów
		rest_->que_.cash_iter_ += rest_->que_.client_meal_.front()->number_;
		
		if (rest_->que_.meas_iter_ >= 200)
		{
			rest_->que_.cash_que_size_ = rest_->que_.cash_iter_*(clock_ - rest_->que_.time_cash_que_);
			rest_->que_.time_all_cash_ += clock_ - rest_->que_.time_cash_que_;
			rest_->que_.time_cash_que_ = clock_;
			cash_que_.push_back(rest_->que_.cash_que_size_);
		}
		rest_->que_.client_meal_.pop_front();
	}
	if (id_ == "Buffet") //koniec konsumpcji w bufecie
	{
		if (ch_ == "s")
		{
			std::cout << "Press any key to continue" << std::endl;
			std::cin >> x_;
			std::cout << "Clients end consumption in buffet" <<std::endl;
		}
		file_ << "Clients end consumption in buffet" << std::endl;
       if(ch_=="s")std::cout << "Clients with id "<<rest_->que_.client_in_buffet_.front()->id_<<" left buffet and go to cashiers que" << std::endl;
		file_<< "Clients with id "<< rest_->que_.client_in_buffet_.front()->id_<<" left buffet and go to cashiers que" << std::endl;
		rest_->que_.client_cash_.push_back(rest_->que_.client_in_buffet_.front()); //klienci id¹ do kasjerów
		rest_->que_.cash_iter_ += rest_->que_.client_in_buffet_.front()->number_; //dane do statystyk
		if (rest_->que_.meas_iter_ >= 200)
		{
			rest_->que_.cash_que_size_ = rest_->que_.cash_iter_*(clock_ - rest_->que_.time_cash_que_);
			rest_->que_.time_all_cash_ += clock_ - rest_->que_.time_cash_que_;
			rest_->que_.time_cash_que_ = clock_;
			cash_que_.push_back(rest_->que_.cash_que_size_);
			
		}
		rest_->que_.client_in_buffet_.pop_front(); //usuniêcie klientów z bufetu
	
	}

	if (id_ == "End")
	{

		   std::list<Cashiers*>::iterator it_ = rest_->cashier_.begin();
		   for (it_; it_ != rest_->cashier_.end(); it_++) //znajdowanie kasjera który jeszcze obs³uguje kilenta
		   {
			   if ((*it_)->cl_ != nullptr) break;
		   }

		    
			//  klienci opuszczaj¹ restauracjê, proces jest skoñczony
			if (ch_ == "s")
			{
				
				std::cout << "Cahier is free" << std::endl;
				std::cout << "Clients with id "<< (*it_)->cl_->id_<<" pay and left restaurant" << std::endl;
			}
			file_ << "Cahier is free" << std::endl;
			file_ << "Clients with id "<< (*it_)->cl_->id_<<" pay and left restaurant" << std::endl;
			rest_->que_.cl_iter_ -= (*it_)->cl_->number_;
			file2_ << rest_->que_.cl_iter_ << std::endl;
			(*it_)->cl_=nullptr; //kasjer znów wolny
	}
	
}

void Event::alarm(std::deque<Clients*>& q_, Generator * gen_, std::string ch_, std::fstream &file_, Restaurant *rest_, std::fstream &file2_)
{
	for (int i_ = 0; i_ < q_.size(); i_++) // wylosowanie klinetów którzy opuszcz¹ restauracjê
	{
		if (gen_->uniform(100, 0) <= 70)
		{
			std::deque<Clients*>::iterator it_ = q_.begin() + i_;
			if (ch_ == "s") std::cout << "Clients with id " << (*it_)->id_ << " left restaurant" <<std::endl;
			rest_->que_.cl_iter_ -= (*it_)->number_;
			rest_->que_.meas_iter_++;
			file2_ << rest_->que_.cl_iter_ << std::endl;
			file_ << "Clients with id " << (*it_)->id_ << " left restaurant" << std::endl;
			if ((*it_)->num_tab2_ != 0) for (int j = 0; j < (*it_)->num_tab2_; j++) rest_->table2_.push_back(new Tables(2));
			if ((*it_)->num_tab3_ != 0) for (int j = 0; j < (*it_)->num_tab3_; j++) rest_->table2_.push_back(new Tables(3));
			if ((*it_)->num_tab4_ != 0) for (int j = 0; j < (*it_)->num_tab4_; j++) rest_->table2_.push_back(new Tables(4));
			delete (*it_);
			q_.erase(it_);
			
		}
	}
}

Event::Event(double time_, std::string a_, double clock_) : event_trig_(true), id_(a_)
{
	event_time_ = abs(time_) + abs(clock_);
	
}


Event::~Event()
{
}
