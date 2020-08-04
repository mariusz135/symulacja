#pragma once
#include "cashiers.h"
#include "waiters.h"
#include "tables.h"
#include <queue>
#include "queue.h"
#include <list>
#include "manager.h"

class Restaurant //klasa implenemtuj�ca restauracj�
{
	
public:

	const int table_2_ = 5;  //ilo�� stolik�w dwuosobowych;
	const int table_3_ = 12;  //ilo�� stolik�w trzyosobowych;
	const int table_4_ = 6;  //ilo�� stolik�w czteroosobowych;
	const int waiters_num_ = 7; //liczba kelner�w
	const int places_in_bufet_ = 18; // liczba miejsc w bufecie
	const int cashier_num_ = 5; //liczba kasjer�w
	double p1_=0.1; //prawdopodobie�stwa przyj�cia grup odpowiednio jedno-, dwu-, trzy- i czteroosobowych
	double p2_=0.1;
    double p3_=0.4;
    double p4_=0.4;
    double prop_buffet_=0.5; //prawdopodobie�stwo p�j�cia do bufetu
    const int u_a_ = 300;// �rednia i wariancja zmiennej losowej m�wi�cej o pojawianiu si� grup klient�w
	const int b_a_ = 50;
	const int u_b_ = 3700;//�rednia i wariancja zmiennej losowej m�wi�cej o czasie sp�dzonym w bufecie
	const int b_b_ = 200;
	const int u_e_ = 4200;//�rednia i wariancja zmiennej losowej m�wi�cej o czasie wyst�pienia alarmu
	const int b_e_ = 50;
	const int lambda_n_ = 400; //�rednie czasu podania napoj�w, dania,konsumpcji, obs�ugi przez kasjera
	const int lambda_j_ = 2150;
	const int lambda_f_ = 2400;
    const int lambda_p_ = 300;
	Queue que_; //obiekt klasy Queue(kolejka)
	Manager *manag_; //wska�nik na menad�era
	std::list<Waiters*> waiter_; //lista kelner�w
	std::list<Cashiers*> cashier_; // lista kasjer�w
	std::list<Tables*> table2_; //lista stolik�w dwuosobowych
	std::list<Tables*> table3_; //lista stolik�w trzyosobowych
	std::list<Tables*> table4_; //lista stolik�w czteroosobowych
	
	Restaurant();
	~Restaurant();
	

};

