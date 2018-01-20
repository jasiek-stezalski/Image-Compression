#pragma once
#include "ObslugaPliku.h"
#include "Huffman.h"
#include <stdlib.h>

class ByteRun : public ObslugaPliku
{
private:
	fstream plik, plik2, plik3;

public:
	bool ma_byc_czarnobialy;
	short padding;

	ByteRun();
	void ByteRunStart();
	void ByteRunKompresuj();
	void ByteRunDekompresuj();

	void WczytajBitmapeDoBufora(fstream &plik, RGB *&pixele);
	bool czyTenSamKolor(RGB *pixele, int index);
	void ByteRunCompress(RGB *pixele, vector <signed char> &bufwyj);
	void ZapisByteRun(fstream &plik2, vector <signed char> &bufwyj);

	long long Odczytztmp(fstream &plik2, vector <char> &bufwej);
	void ByteRunDecompress(vector <char> &bufwej, unsigned char *&bufdecompressed, long long dlugosc);
	void ByteRun_ZapisDoBMP(fstream &plik3, unsigned char* bufdecompressed);


	void ColorToGray(RGB *&pixele);

	template <typename Typ>
	void TworzenieTablicy(Typ *&tab, long long a);


	//	void TworzenieTablicy(RGB *&tab, long long a);
	//	void TworzenieTablicy(char *&tab, long long a);
	//	void TworzenieTablicy(unsigned char *&tab, long long a);
};

template <typename Typ>
void ByteRun::TworzenieTablicy(Typ *&tab, long long a)
{

	try
	{
		tab = new Typ[a];
	}
	catch (bad_alloc)
	{
		cout << "Brak miejsca na utworzenie tablicy RGB";
		delete[] tab;
		cin.get();
		exit(0);
	}


}
