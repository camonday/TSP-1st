#pragma once
#include <string>

class MyList
{
    struct Neighbour
    {
        int id = 1; //=1 w celu sprawdzenia czy listia sasiadow jest zapoczatkowana
                    //PATRZ: MyList.cpp metoda addNeighbour
        int distance;
        Neighbour* next;
    };
    struct ElemList {
        int id;
        Neighbour* data;
        ElemList* prev;
        ElemList* next;
    };

    ElemList* head = NULL;  //na poczatku lista jest pusta wiec nie ma ani head ani tail
    ElemList* tail = NULL;
    ElemList* temp = NULL;
    Neighbour* tempN = NULL;
    int maxIndex = 0; //maksymalny indeks jaki mozna dodac




    ElemList* look4Index(int index);
    void addValue(int index);
    void addNeighbour(int index, int distance);
    void deleteFromList_byIndex(int index);

public:
    //Overiden:
    void display();
    int loadFromFile(const std::string& FileName);
    void generate(int size);
    void clear();
    int findDistance(int A, int B);
    int size();
    int getMinDistance(int A);

};
   