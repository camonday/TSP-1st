#pragma once
#include <queue>
#include <list>
#include "Reprezentacja.h"
#include "Algorytm.h"


	struct Przystanek {
		int id;
		int waga;
		int lower_bound;
		std::list<int> sciezka;
		std::list <int> jno; //Jeszcze Nie Odwiedzeni
	};

class BnB : public Algorytm {
	
private:
	 std::queue <Przystanek*> obecny_poziom;
	 std::queue <Przystanek*> następny_poziom;
	 int startoweMiasto;
	 Przystanek* badany;
	 Reprezentacja* miasta;
	
	 void OdwiedzPrzystanek(); //odwiedza badanego
	 void idzDo_NastepnyPoziom();
	 int PoliczLB(Przystanek przystanek);
	 void ZnajdzUB();

public:
	 int ZnajdzNajlepsze(Reprezentacja* przekazaneMiasta) override;
};
