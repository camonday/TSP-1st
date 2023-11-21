#pragma once
#include <queue>
#include <list>
#include "Reprezentacja.h"
#include "Algorytm.h"


	struct Przystanek {
		int id = -1;
		int waga = 0;
		int lower_bound = 0;
		std::list<int> sciezka;
		std::list <int> jno; //Jeszcze Nie Odwiedzeni

	};

class BnB : public Algorytm {
	
private:
	 std::queue <Przystanek> obecny_poziom;
	 std::queue <Przystanek> następny_poziom;
	 int startoweMiasto;
	 int new_deleteCounter;
	 Przystanek badany;
	 Reprezentacja* miasta;
	
	 void OdwiedzPrzystanek(); //odwiedza badanego
	 void idzDo_NastepnyPoziom();
	 int PoliczLB(Przystanek przystanek);
	 void ZnajdzUB();

public:
	 int ZnajdzNajlepsze(Reprezentacja* przekazaneMiasta) override;
};
