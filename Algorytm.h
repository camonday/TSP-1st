#pragma once
#include "Reprezentacja.h"
#include <list>

class Algorytm 
{
protected:
	std::list <int> sciezka_UB;
	int waga_UB;
	Reprezentacja* miasta;
public:
	virtual int ZnajdzNajlepsze(Reprezentacja* przekazaneMiasta);
	void showWynik();
};
