#include <iostream>
#include <fstream>
#include <queue>

#ifndef HUFFMAN_H
#define HUFFMAN_H

#include "ObslugaPliku.h"

using namespace std;

class Huffman : ObslugaPliku
{
	struct drzewo
	{
		int Hz;
		int wartosc;
		drzewo *lewy;
		drzewo *prawy;
	};

	struct porownanie_Hz
	{
		bool operator ()(const drzewo & wartosc1, const drzewo &wartosc2)
		{
			return wartosc1.Hz > wartosc2.Hz;
		};
	};

	struct tablica
	{
		string kod;
		int liczba;
	};

	typedef priority_queue < drzewo, vector < drzewo >, porownanie_Hz > kolejkaznakow;

	kolejkaznakow kolejkaPriorytetowa;
	fstream plik, plik2, plik3;
	enum { N = 128 };
	string tabKod[N];

	// Funkcje do algorytmu Huffmana
	void Wstawznak(int wartosc, float Hz);
	void Odczyt(drzewo *odczytana, string s);
	void Korzen(drzewo *&odczytana1, drzewo *&odczytana2);
	void UsunDrzewo(drzewo *&korzen);

	// Funckje do kompresji
	int BinToDec(string bin);
	string DecToBin(int co);

	// Funckja do tworzenia tablicy
	void TworzenieTablicy(tablica *&tab, int a);

public:

	// Funkcje ogolne
	void OdczytBMP(bool szary);
	void Zliczanie();
	void AlgorytmHuffmana();
	void NaszFormat();
	void Kompresja();
	void Dekompresja();
};

#endif
