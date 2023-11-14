#pragma once
#include "Reprezentacja.h"

class Algorytm 
{
public:
	virtual int ZnajdzNajlepsze(Reprezentacja* przekazaneMiasta);
	virtual void showWynik();
};
