#pragma once
#include <string>
#include <vector>

class Reprezentacja
{
    
    int size = 0; //maksymalny indeks jaki mozna dodac
    std::vector<std::vector<int>> dane;

public:
    //Overiden:
    
    int loadFromFile(const std::string& FileName);
    void generate(int size);
    void display();
    void clear();
    int getSize();

    int findDistance(int A, int B);
    int getMinDistance(int A);

};
   