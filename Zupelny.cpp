#include "Zupelny.h"
#include <algorithm>

int Zupelny::ZnajdzNajlepsze(Reprezentacja* przekazaneMiasta)
{
    miasta = przekazaneMiasta;
    int size = miasta->getSize();
    int wagaTemp;
    std::vector<int> sciezka;
    sciezka_UB.clear();
    waga_UB = miasta->findDistance(size - 1, 0);

    for (int i = 0; i < size; i++)
    {
        sciezka.push_back(i);
        sciezka_UB.push_back(i);
        if (i < size - 1) {
            waga_UB += miasta->findDistance(i, i + 1);
        }
    }
    sciezka_UB.push_back(0);

    while (std::next_permutation(sciezka.begin(), sciezka.end())) {
        wagaTemp = 0;
        for (int i = 0; i < size-1; i++){
            wagaTemp += miasta->findDistance(sciezka[i], sciezka[i + 1]);
        }
        wagaTemp += miasta->findDistance(sciezka[size-1], sciezka[0]);

        if (wagaTemp < waga_UB) {
            waga_UB = wagaTemp;
            sciezka_UB.clear();
            for (int i = 0; i < size; i++)
            {
                sciezka_UB.push_back(sciezka[i]);
            }
            sciezka_UB.push_back(sciezka[0]);
        }
    }

    return 0;
}

