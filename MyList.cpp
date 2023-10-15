#include "MyList.h"
#include <iostream>
#include <fstream>

void MyList::display() { //wyswietlanie listy od przodu i tylu
    temp = head;
    while (temp != NULL)
    {   
        std::cout << "Odległość od miasta " << temp->id << " do miasta: \n";
        tempN = temp->data;
        while (tempN->id >= 0)
        {
            std::cout << tempN->id << " " << tempN->distance<<"km\n";
            tempN = tempN->next;
        }
        temp = temp->next;
    }
    std::cout << "\n";
}

void MyList::addValue(int index) { //dodawanie wartosci

    ElemList* newbie = new ElemList;
    newbie->id = index;
    ElemList* nextie = look4Index(index);


    newbie->next = nextie; //newbie is gonna know, he is b4 nextie
    if (nextie != NULL) { //jesli nie dodalismy ogona
        newbie->prev = nextie->prev;
        nextie->prev = newbie;
    }
    else {//jesli dodalismy ogon
        if (tail != NULL) tail->next = newbie;
        newbie->prev = tail;
        tail = newbie;
    }

    if (newbie->prev != NULL) newbie->prev->next = newbie; //jesli nie dodalismy glowy
    else {//jesli dodalismy glowe
        head = newbie;
    }

    maxIndex++; //zwiekszamy ilosc elementow w tablicy

}



MyList::ElemList* MyList::look4Index(int index) {
    temp = head;
    if (maxIndex < index) {
        //std::cout<<"\nIndex too big, I look for biggest possible index\n";
        index = maxIndex;
    }
    for (int i = 0; i < index; i++) {
        temp = temp->next;
    }
    //temp jest tym co szukalismy
    return temp;
}

void MyList::addNeighbour(int index, int distance)
{
    Neighbour* neighbour = new Neighbour;
    ElemList* startingCity = look4Index(index);
    neighbour = startingCity->data;
    if (neighbour->id == 0){ //
        while (neighbour->id >= 0)
        {
            neighbour = neighbour->next;
        }
        neighbour->id *= -1;
    }
    else {
        look4Index(index)->data = neighbour;
        neighbour->id = 0;
        
    }
    neighbour->next = new Neighbour;
    neighbour->distance = distance;
    neighbour->next->id = -1 * (neighbour->id + 1);
    //std::cout << "dodalem " << neighbour->id<<"\n";
}



void MyList::deleteFromList_byIndex(int index) {
    if (index >= 0) look4Index(index); //jesli index =-1, to temp jest juz ustawiony na wynik look4value, w przeciwnym wypadku ustawimy go na wynik
    // look4Index
    if (temp != NULL) {

        if (temp->next != NULL) { //jesli temp ma potomka to ten potomek stoi teraz za tym za czym stal temp a nie za temp
            temp->next->prev = temp->prev;
        }
        else {
            tail = temp->prev; //jesli nie ma potomka to byl ogonem wiec teraz ogonem bedzie to co stalo przed temp
        }

        if (temp->prev != NULL) { //analogicznie jesli temp ma/nie ma rodzica
            temp->prev->next = temp->next;
        }
        else {
            head = temp->next;
        }
        maxIndex--; //zmniejszamy liczbe elementów listy

        //clear memory
        delete temp;
        temp = NULL;

    }
    else { //nie znalezlismy wartosci która mamy usunac
        std::cout << "Nothing to delete " << maxIndex;
    }
}

void MyList::generateList(int size) {
    clearList(); //czyscimy liste
    for (int i = 0; i < size; i++) {
        addValue(maxIndex);
        Neighbour* neighbour = new Neighbour;
        look4Index(maxIndex - 1)->data = neighbour;
        for (int j = 0; j < size; j++) {
            int dist = -1;
            if (j != i) dist = rand();
            addNeighbour(maxIndex - 1, dist);
        }
    }
}

void MyList::clearList() {
    while (maxIndex != 0) { //usuwamy kazdy element z kisty tak dlugo, az lista ma zero elementow
        deleteFromList_byIndex(maxIndex - 1);
    }
}

int MyList::findDistance(int A, int B)
{
    tempN = look4Index(A)->data;
    while (tempN->id != B) tempN = tempN->next;
    return tempN->distance;
}

int MyList::size()
{
    return (tail->id)+1;
}

int MyList::loadFromFile(const std::string& FileName) {
    clearList(); //czyscimy liste
    std::fstream file;
    file.open(FileName);

    int size, distance;
    
    file >> size; //pierwsza liczba w pliku to rozmiar listy

    while (size != maxIndex) {
        addValue(maxIndex); //addvalue zwieksza maxIndex wiec petla zatrzyma sie
        Neighbour* neighbour = new Neighbour;
        look4Index(maxIndex - 1)->data = neighbour;
        for (int i = 0; i < size; i++) {
            file >> distance;
            addNeighbour(maxIndex - 1, distance);
        }
    }

    file.close();
    return 0;
}