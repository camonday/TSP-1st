#include "BnB.h"
#include <iostream>

void BnB::OdwiedzPrzystanek()
{
	//std::cout << "\n\n Nowy badany: " << badany->id<<" sasiedzi: ";
	for (int& sasiad : badany->jno)
	{
		
		//stworz przystanek
		Przystanek* nowy = new Przystanek();
		nowy->id = sasiad;
		//std::cout << "\nbadany: "<<badany->id<<" sasiad: "<<sasiad;
		nowy->waga = (badany->waga) + miasta->findDistance(badany->id, nowy->id);

		//utworz liste jeszcze nie odwiedzonych
		for (int& nie_odwiedzony : badany->jno) {
			if (nie_odwiedzony != nowy->id) {
				nowy->jno.push_back(nie_odwiedzony);
			}
		}

		//utworz sciezke ktora doprowadzila do tego punktu
		for (int& odwiedzony : badany->sciezka) {
			nowy->sciezka.push_back(odwiedzony);
		}
		nowy->sciezka.push_back(nowy->id);

		//jesli przedostatni przystanek to policz juz wage calej sciezki
		if (nowy->jno.size() == 1) {
			
			nowy->waga += miasta->findDistance(nowy->id, nowy->jno.front());
			nowy->waga += miasta->findDistance(nowy->jno.front(), startoweMiasto);

			
			//jesli cala sciezka jest lepsza niz dotychczasowy najlepszy to mamy nowego najlepszego
			if (nowy->waga <= waga_UB) {
				//std::cout << sasiad << ", ";
				waga_UB = nowy->waga;
				for (int& przystanek : nowy->sciezka) {
					sciezka_UB.push_back(przystanek);
				}
				sciezka_UB.push_back(nowy->jno.front());
				sciezka_UB.push_back(startoweMiasto);
			}

			//sprawdzilismy jedyna sciezke ktora wychodzi z nowego - mozemy go usunac
			delete nowy;
		}
		//W przeciwnym wypadku sprawdz czy sciezki od niego wychodzace maja szanse byc lepsze od obecnie najlepszego
		else {
			nowy->lower_bound = PoliczLB(*nowy);
			if (nowy->lower_bound <= waga_UB) {
				następny_poziom.push(nowy); //std::cout << sasiad << ", ";
			}
			else delete nowy;
		}
		
	}
	
}

void BnB::idzDo_NastepnyPoziom()
{
	//std::cout << "\n\nNowyPoziom!!!";
	//wezly z nastepnego poziomu przechodza na liste obecnego poziomu
	obecny_poziom.swap(następny_poziom);

	//upewniamy sie, ze nastepny poziom jest gotowy na przyjmowanie nowych danych
	while (następny_poziom.size() > 0) {
		następny_poziom.pop();
	}
}

int BnB::PoliczLB(Przystanek przystanek)
{
	int suma = przystanek.waga;
	for (int& sasiad : przystanek.jno)
	{
		suma += miasta->getMinDistance(sasiad);
	}
	suma += miasta->getMinDistance(startoweMiasto);
	return suma;
}

void BnB::ZnajdzUB()
{
	int badany=startoweMiasto;
	int waga_pojedyncza=0;
	int nastepny;

	// lista nieodwiedzonych sasiadow
	std::list<int> nieodwiedzeni;
	for (int i = 1; i < miasta->size(); i++)
	{
		nieodwiedzeni.push_back(i);
	}
	sciezka_UB.push_back(startoweMiasto);
	while (nieodwiedzeni.size() > 0) {
		waga_pojedyncza = RAND_MAX;
		for (int& sasiad : nieodwiedzeni) {
			//sprawdz kazdego nieodwiedzonego jeszcze sasiada i wybierz najlepszego
			if (waga_pojedyncza > miasta->findDistance(badany, sasiad)) {
				waga_pojedyncza = miasta->findDistance(badany, sasiad);
				nastepny = sasiad;
			}
		}

		//usun najlepszego sasiada z nieodwiedzonych
		nieodwiedzeni.remove(nastepny);
		badany = nastepny;

		//uaktualnij dane UB
		waga_UB += waga_pojedyncza;
		sciezka_UB.push_back(nastepny);
	}
	// dodaj polaczenie wracajae do miasta startowego
	waga_UB += miasta->findDistance(badany, startoweMiasto);
	sciezka_UB.push_back(startoweMiasto);

}

int BnB::ZnajdzNajlepsze(MyList* przekazaneMiasta)  
{	
	miasta = przekazaneMiasta;
	startoweMiasto = 0;
	//2) upper bound
	ZnajdzUB();

	//3) budowa drzewa
	//stworz przystanek startowy
	Przystanek* nowy = new Przystanek();
	nowy->id = startoweMiasto;
	for (int i = 1; i < miasta->size(); i++) {
		nowy->jno.push_back(i);
	}
	nowy->waga = 0;
	nowy->sciezka.push_back(startoweMiasto);

	//dodaj przystanek do listy do odwiedzenia
	następny_poziom.push(nowy);

	while (!(następny_poziom.empty())){
		idzDo_NastepnyPoziom();
		while (!(obecny_poziom.empty()))
		{
			badany = obecny_poziom.front();
			OdwiedzPrzystanek();
			obecny_poziom.pop();
			delete badany;
		}
	}

	return waga_UB;
}

void BnB::showWynik()
{
	std::cout << "\nwaga: " << waga_UB << "\nścieżka: ";
	for (int& przystanek : sciezka_UB) {
		std::cout << przystanek;
	}
}
