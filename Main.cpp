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

void testy(string nazwa, Algorytm* algorytm, bool czyDopuszczalnyCzas);
void testyB(string nazwa);

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
                delete algortmBnB;
                algortmBnB = new BnB();

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
                testy("Zupelny_test.txt", algorytmZupelny, false);
                break;
            case '2':
                testyB("BnB_test_1s.txt");
                break;
            }
            break;
        }

    } while (option != '0');


    return 0;
}





void testy(string nazwa, Algorytm* algorytm, bool czyDopuszczalnyCzas)
{
    std::ofstream file(nazwa);
    file.imbue(std::locale(std::locale::classic(), new Comma));
    int odrzucone = 0;
    int dopuszczalnyCzas = 1;


    for (int n = 13; n<14; n++) {
        cout << n <<" ";
        timeSum -= timeSum;
        odrzucone = 0;
        for (int i = 1; i < 100 ; i++) {
            miasta.clear();
            miasta.generate(n);

            // --- pomiar czasu
            timeStart = steady_clock::now();

            algorytm->ZnajdzNajlepsze(&miasta);

            timeEnd = steady_clock::now();

            timeTemp = duration_cast<duration<double>>(timeEnd - timeStart);
            if (czyDopuszczalnyCzas && (timeTemp.count() > dopuszczalnyCzas)) {
                odrzucone++;
            }
            else {
                timeSum += timeTemp;
            }
            
        }

        file << n << " " << timeSum.count() << " "<< odrzucone <<"\n";

    }
    timeSum -= timeSum;
    file.close();
}

void testyB(string nazwa)
{
    std::ofstream file(nazwa);
    file.imbue(std::locale(std::locale::classic(), new Comma));
    int odrzucone = 0;
    int dopuszczalnyCzas = 6000;


    for (int n = 5; n<=15; n+=1) {
        cout <<"\n"<< n << " ";
        timeSum -= timeSum;
        odrzucone = 0;
        for (int i = 1; i < 100 ; i++) {
            miasta.clear();
            miasta.generate(n);
            cout << ".";

            delete algortmBnB;
            algortmBnB = new BnB();
            // --- pomiar czasu
            try {
            timeStart = steady_clock::now();

            algortmBnB->ZnajdzNajlepsze(&miasta);

            timeEnd = steady_clock::now();
            }
            catch (std::exception e) {
                odrzucone++;
                timeEnd = timeStart;
                //cout << "\n Zbyt trudne\n";
            }

            timeTemp = duration_cast<duration<double>>(timeEnd - timeStart);
           
            if (timeTemp.count() < dopuszczalnyCzas) {
                timeSum += timeTemp;
            }
            else {
               odrzucone++;
               cout << "\n Czas\n";
            }

        }

        file << n << " " << timeSum.count() << " " << odrzucone << "\n";
        std::cout << " time of 100 " << timeSum.count();
    }
    timeSum -= timeSum;
    file.close();
}

