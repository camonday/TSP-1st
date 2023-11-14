#include "Algorytm.h"
#include <iostream>

int Algorytm::ZnajdzNajlepsze(Reprezentacja* przekazaneMiasta)
{
    std::cout << "nie wybrales algorytmu";
    return 0;
}

void Algorytm::showWynik()
{
	std::cout << "\nwaga: " << waga_UB << "\nścieżka: ";
	for (int& przystanek : sciezka_UB) {
		std::cout << przystanek << " ";
	}
}
