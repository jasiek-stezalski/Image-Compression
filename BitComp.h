#pragma once
#include "ObslugaPliku.h"
#include "Huffman.h"
#include "cstdlib"

class BitComp : public ObslugaPliku
{
private:
	fstream plik, plik2, plik3;

public:
	bool ma_byc_czarnobialy;

	BitComp();
	void BitCompStart();
	void BitCompKompresuj(vector <signed char> &bufwyj);
	void BitCompDekompresuj();
	void WczytajBitmape(vector <signed char> &bufwyj);
	void ZapisBitComp(fstream &plik2, vector <signed char> &bufwyj);
	void BitComp_ZapisDoBMP(fstream &plik3, vector <signed char> &bufwej);
};
