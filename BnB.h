#pragma once
#include <queue>
#include <list>
#include "MyList.h"
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
	 std::list <int> sciezka_UB;
	 int waga_UB;
	 int startoweMiasto;
	 Przystanek* badany;
	 MyList* miasta;
	
	 void OdwiedzPrzystanek(); //odwiedza badanego
	 void idzDo_NastepnyPoziom();
	 int PoliczLB(Przystanek przystanek);
	 void ZnajdzUB();

public:
	 int ZnajdzNajlepsze(MyList* przekazaneMiasta) override;
	 void showWynik() override;
};
