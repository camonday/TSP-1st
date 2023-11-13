#include<conio.h>
#include<string>
#include<iostream>
#include <fstream>
#include <chrono>
#include <windows.h>
#include <vector>
#include "MyList.h"
#include "BnB.h"

double PCFreq = 0.0;
__int64 CounterStart = 0;

void StartCounter()
{
    LARGE_INTEGER li;
    if (!QueryPerformanceFrequency(&li))
        std::cout << "QueryPerformanceFrequency failed!\n";

    PCFreq = double(li.QuadPart);

    QueryPerformanceCounter(&li);
    CounterStart = li.QuadPart;
}
double GetCounter()
{
    LARGE_INTEGER li;
    QueryPerformanceCounter(&li);
    return double(li.QuadPart - CounterStart) / PCFreq;
}
struct WynikB {
    int suma = INT_MAX;
    int zmienna = -1;
};
WynikB brute(MyList miasta, int ileMiast);
int silnia(int n);
void showWynikB(WynikB wynik, int ileMiast);

using namespace std;
using namespace std::chrono;

struct Comma final : std::numpunct<char>
{
    char do_decimal_point() const override { return ','; }
};

void displayMenu(const string& info)
{
    std::cout << std::endl;
    std::cout << info << std::endl;
    std::cout << "1.Wczytaj z pliku" << std::endl;
    std::cout << "2.Utworz losowo" << std::endl;
    std::cout << "3.Wyswietl" << std::endl;
    std::cout << "4.Test (pomiary)" << std::endl;
    std::cout << "0.Powrot do menu" << std::endl;
    std::cout << "Podaj opcje:";
}

steady_clock::time_point timeStart, timeEnd;
duration<double> timeTemp, timeSum;
MyList miasta; 
int aaa;
string fileName;
BnB* algortmBnB = new BnB();

void menu_brute()
{
    char opt;
    
    int index, value;
    int trial = 100, population;
    WynikB wynik;
   
    

    do {
        displayMenu("--- PRZEGLAD ZUPELNY ---");
        opt = _getche();
        std::cout << std::endl;
        switch (opt) {
        case '1': //tutaj wczytytwanie danych z pliku
            std::cout << " Podaj nazwę zbioru:";
            std::cin >> fileName;
            miasta.loadFromFile(fileName);
            miasta.display();
            //brute
            
            timeStart = steady_clock::now();

            wynik = brute(miasta, miasta.size());
            //aaa = algortmBnB->ZnajdzNajlepsze(&miasta);

            timeEnd = steady_clock::now();

            timeTemp = duration_cast<duration<double>>(timeEnd - timeStart);
            
            std::cout << "\n\nROZWIAZANIE\n" /* << aaa*/;
            showWynikB(wynik, miasta.size());
            std::cout << "\n czas: " << timeTemp.count() << " sekund";
            break;

        case '2':  //tutaj generowanie danych
            std::cout << "Podaj ilość miast:";
            std::cin >> value;
            miasta.generate(value);
            miasta.display();
            timeStart = steady_clock::now();

            wynik = brute(miasta, miasta.size());

            timeEnd = steady_clock::now();

            timeTemp = duration_cast<duration<double>>(timeEnd - timeStart);

            std::cout << "\n\nROZWIAZANIE\n";
            showWynikB(wynik, miasta.size());
            std::cout << "\n czas: " << timeTemp.count() << " sekund";

            break;

        case '3':  //tutaj wyświetlanie danych
            miasta.display();
            break;

        case '4': //tutaj nasza funkcja do eksperymentów (pomiary czasów i generowanie daneych) - nie będzie testowana przez prowadzącego
            // można sobie tu dodać własne case'y


            std::ofstream file("test_brute1.txt");
            file.imbue(std::locale(std::locale::classic(), new Comma));
            int* values = new int[trial];
            int* indexes = new int[trial];
            for (int n = 11; timeSum.count()<=300; n++) {
                cout << n;
                timeSum -= timeSum;
                for (int i = 1; i < 100 && timeSum.count() <= 300; i++) {
                    miasta.clear();
                    miasta.generate(n);

                    //std::cout << std::endl << i;
                    timeStart = steady_clock::now();

                    brute(miasta, n);

                    timeEnd = steady_clock::now();

                    timeTemp = duration_cast<duration<double>>(timeEnd - timeStart);
                    timeSum += timeTemp;
                    //std::cout << " timeTemp " << timeTemp.count();
                }

                file << n <<" " << timeSum.count()<<"\n";

            }
        }

    } while (opt != '0');
}

int main(int argc, char* argv[])
{

    char option;
    do {
        std::cout << std::endl;
        std::cout << "==== MENU GLOWNE ===" << std::endl;
        std::cout << "1.Brute force" << std::endl;
        std::cout << "2.BnB" << std::endl;
        std::cout << "3.---" << std::endl;
        std::cout << "0.Wyjscie" << std::endl;
        std::cout << "Podaj opcje:";
        option = _getche();
        std::cout << std::endl;

        switch (option) {
        case '1':
            menu_brute();
            break;

        case '2':
           // menu_BnB();
            std::cout << " Podaj nazwę zbioru:";
            std::cin >> fileName;
            miasta.loadFromFile(fileName);
            miasta.display();
            

            timeStart = steady_clock::now();

            
            aaa = algortmBnB->ZnajdzNajlepsze(&miasta);

            timeEnd = steady_clock::now();

            timeTemp = duration_cast<duration<double>>(timeEnd - timeStart);

            std::cout << "\n\nROZWIAZANIE\n"  << aaa;
            std::cout << "\n czas: " << timeTemp.count() << " sekund";
            break;
            break;

        case '3':
            break;
        }

    } while (option != '0');


    return 0;
}

WynikB brute(MyList miasta, int ileMiast)
{
    
    WynikB wynik;
    int S = silnia(ileMiast - 1);
    for (int x = 1; x <= S; x++)
    {
        //cout << "\n\n" << x<<"\n";
        //stworz liste sasiadow
        std::vector<int> sasiad_list;
        for (int i = 0; i < ileMiast; i++) sasiad_list.push_back(i);
        vector<int>::iterator it;

        int obecne_id = 0;
        it = sasiad_list.begin();
        sasiad_list.erase(it); //odwiedzilismy juz miasto 0 wiec je usuwamy

        int next_id = 0;
        int suma = 0;
        int pozycja;
        
        while (!(sasiad_list.empty())) {
            pozycja = (obecne_id + x) % sasiad_list.size();
            next_id = sasiad_list.at(pozycja);

            suma += miasta.findDistance(obecne_id, next_id);
            if (suma >= wynik.suma) break;

            obecne_id = next_id;
            it = sasiad_list.begin();
            it += pozycja;
            sasiad_list.erase(it);
            //cout << " .";
        }
        suma += miasta.findDistance(obecne_id, 0);
        //cout << "\nSUMA: " << suma;
        //showWynikB({ suma, x }, ileMiast);
        if(suma<wynik.suma){
            wynik.suma = suma;
            wynik.zmienna = x;
            //cout << "  - nowy najlepszy";
        }
    }
    return wynik;
}

int silnia(int n) {
    int wynik = 1;
    while (n)
        wynik *= n--;
    return wynik;
}

void showWynikB(WynikB wynik, int ileMiast)
{
    std::vector<int> sasiad_list;
    for (int i = 0; i < ileMiast; i++) sasiad_list.push_back(i);
    vector<int>::iterator it;

    int obecne_id = 0;
    it = sasiad_list.begin();
    sasiad_list.erase(it);

    int next_id = 0;
    int suma = 0;
    int pozycja;

    std::cout << obecne_id << " ";
    while (!(sasiad_list.empty())) {
        pozycja = (obecne_id + wynik.zmienna) % sasiad_list.size();
        next_id = sasiad_list.at(pozycja);

        it = sasiad_list.begin();
        it += pozycja;
        sasiad_list.erase(it);

        
        obecne_id = next_id;
        std::cout << obecne_id << " ";
    }
    std::cout << 0 << " ";
    cout << "\nSUMA: " << wynik.suma;
}
