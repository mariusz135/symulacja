#pragma once
#include "cashiers.h"
#include "waiters.h"
#include "tables.h"
#include <queue>
#include "queue.h"
#include <list>
#include "manager.h"

class Restaurant //klasa implenemtuj¹ca restauracjê
{
	
public:

	const int table_2_ = 5;  //iloœæ stolików dwuosobowych;
	const int table_3_ = 12;  //iloœæ stolików trzyosobowych;
	const int table_4_ = 6;  //iloœæ stolików czteroosobowych;
	const int waiters_num_ = 7; //liczba kelnerów
	const int places_in_bufet_ = 18; // liczba miejsc w bufecie
	const int cashier_num_ = 5; //liczba kasjerów
	double p1_=0.1; //prawdopodobieñstwa przyjœcia grup odpowiednio jedno-, dwu-, trzy- i czteroosobowych
	double p2_=0.1;
    double p3_=0.4;
    double p4_=0.4;
    double prop_buffet_=0.5; //prawdopodobieñstwo pójœcia do bufetu
    const int u_a_ = 300;// œrednia i wariancja zmiennej losowej mówi¹cej o pojawianiu siê grup klientów
	const int b_a_ = 50;
	const int u_b_ = 3700;//œrednia i wariancja zmiennej losowej mówi¹cej o czasie spêdzonym w bufecie
	const int b_b_ = 200;
	const int u_e_ = 4200;//œrednia i wariancja zmiennej losowej mówi¹cej o czasie wyst¹pienia alarmu
	const int b_e_ = 50;
	const int lambda_n_ = 400; //œrednie czasu podania napojów, dania,konsumpcji, obs³ugi przez kasjera
	const int lambda_j_ = 2150;
	const int lambda_f_ = 2400;
    const int lambda_p_ = 300;
	Queue que_; //obiekt klasy Queue(kolejka)
	Manager *manag_; //wskaŸnik na menad¿era
	std::list<Waiters*> waiter_; //lista kelnerów
	std::list<Cashiers*> cashier_; // lista kasjerów
	std::list<Tables*> table2_; //lista stolików dwuosobowych
	std::list<Tables*> table3_; //lista stolików trzyosobowych
	std::list<Tables*> table4_; //lista stolików czteroosobowych
	
	Restaurant();
	~Restaurant();
	

};

