#include<conio.h>
#include<string>
#include<iostream>
#include <fstream>
#include <chrono>
#include <windows.h>
#include <vector>
#include "Reprezentacja.h"
#include "BnB.h"
#include "Algorytm.h"
#include "Zupelny.h"

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
WynikB brute(Reprezentacja miasta, int ileMiast);
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
    std::cout << "1.Przegląd Zupełny" << std::endl;
    std::cout << "2.BnB" << std::endl;
    std::cout << "3.---" << std::endl;
    std::cout << "4.---" << std::endl;
    std::cout << "Podaj opcje:";
}

void testy(string nazwa, Algorytm* algorytm);

steady_clock::time_point timeStart, timeEnd;
duration<double> timeTemp, timeSum;
Reprezentacja miasta; 
int rozmiar;
string fileName;
BnB* algortmBnB = new BnB();
Zupelny* algorytmZupelny = new Zupelny();

char menu_algorytm()
{
    char opt;
    displayMenu("---KTÓRY ALGORYTM?---");
    opt = _getche();
    std::cout << std::endl;
    return opt;
}

int main(int argc, char* argv[])
{

    char option;
    char option2;
    do {
        std::cout << std::endl;
        std::cout << "==== MENU GLOWNE ===" << std::endl;
        std::cout << "1.Wczytaj z pliku" << std::endl;
        std::cout << "2.Wylosuj" << std::endl;
        std::cout << "3.Wyświetl" << std::endl;
        std::cout << "4.Znajdź drogę" << std::endl;
        std::cout << "--------------------" << std::endl;
        std::cout << "5.Testy" << std::endl;
        std::cout << "0.Wyjscie" << std::endl;
        std::cout << "Podaj opcje:";
        option = _getche();
        std::cout << std::endl;

        switch (option) {
        case '1':   // 1. Wczytaj z pliku
            std::cout << " Podaj nazwę zbioru:";
            std::cin >> fileName;
            miasta.loadFromFile(fileName);
            miasta.display();
            break;

        case '2':   // 2. Wylosuj
            std::cout << " Podaj ilość miast:";
            std::cin >> rozmiar;
            miasta.generate(rozmiar);

        case '3':   // 3. Wyświetl
            miasta.display();
            break;

        case '4': // 4. Znajdź sciezkę
            //zapytaj jaką metodą
            option2 = menu_algorytm();

            switch (option2) {
            case '1':
                timeStart = steady_clock::now();
                algorytmZupelny->ZnajdzNajlepsze(&miasta);
                timeEnd = steady_clock::now();

                timeTemp = duration_cast<duration<double>>(timeEnd - timeStart);
                std::cout << "\n czas: " << timeTemp.count() << " sekund";
                algorytmZupelny->showWynik();
                break;
            case '2':
                timeStart = steady_clock::now();
                algortmBnB->ZnajdzNajlepsze(&miasta);
                timeEnd = steady_clock::now();

                timeTemp = duration_cast<duration<double>>(timeEnd - timeStart);
                std::cout << "\n czas: " << timeTemp.count() << " sekund";
                algortmBnB->showWynik();
                break;
            }
            break;

        case '5': //5. testy
            //zapytaj o metode
            option2 = menu_algorytm();

            switch (option2) {
            case '1':
                break;
            case '2':
                testy("BnB0_test.txt", algortmBnB);
                break;
            }
            break;
        }

    } while (option != '0');


    return 0;
}

WynikB brute(Reprezentacja miasta, int ileMiast)
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

void testy(string nazwa, Algorytm* algorytm)
{
    std::ofstream file(nazwa);
    file.imbue(std::locale(std::locale::classic(), new Comma));
    int index, value;
    int trial = 100, population;

    int* values = new int[trial];
    int* indexes = new int[trial];
    for (int n = 5; timeSum.count() <= 300; n++) {
        cout << n;
        timeSum -= timeSum;
        for (int i = 1; i < 100 && timeSum.count() <= 300; i++) {
            miasta.clear();
            miasta.generate(n);

            // --- pomiar czasu
            timeStart = steady_clock::now();

            algorytm->ZnajdzNajlepsze(&miasta);

            timeEnd = steady_clock::now();

            timeTemp = duration_cast<duration<double>>(timeEnd - timeStart);
            timeSum += timeTemp;
            //std::cout << " timeTemp " << timeTemp.count();
        }

        file << n << " " << timeSum.count() << "\n";

    }
    file.close();
}

