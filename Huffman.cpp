#include "Huffman.h"
#include <cmath>
#include <string>

//-------------Funkcje ogolne---------------------------------------

void Huffman::OdczytBMP(bool szary)
{
	// Odczyt naglowka pliku bmp

	string picture;

    cout << "Podaj nazwe obrazku jaki chcesz skompresowac: " << endl;
    cin >> picture;

	OdczytNaglowka(picture+".bmp", plik);

	// Tworzenie tablicy na pixele
	TablicaPixeli();

	// Zapis pixeli do tablicy
	OdczytBitmapy(plik, szary);

	plik.close();


}

void Huffman::AlgorytmHuffmana()
{
	for (int i = 0; i < 128; ++i)
		tabKod[i] = "pusty";
	string s;
	drzewo *odczytana1;
	drzewo *odczytana2;
	while (kolejkaPriorytetowa.size() > 1)
	{
		odczytana1 = new drzewo;
		odczytana2 = new drzewo;
		*odczytana1 = kolejkaPriorytetowa.top();
		kolejkaPriorytetowa.pop();
		*odczytana2 = kolejkaPriorytetowa.top();
		kolejkaPriorytetowa.pop();
		Korzen(odczytana1, odczytana2);
	}
	odczytana1 = new drzewo;
	*odczytana1 = kolejkaPriorytetowa.top();
	kolejkaPriorytetowa.pop();
	Odczyt(odczytana1, s);
	UsunDrzewo(odczytana1);
}

void Huffman::NaszFormat()
{
	ZapisNaglowka("Huffman.xd", plik2);
	int licznik = 0;
	for (int i = 0; i<128; ++i)
		if (tabKod[i] != "pusty")
			++licznik;
	plik2 << licznik << " ";
	for (int i = 0; i<128; ++i)
		if (tabKod[i] != "pusty")
		{
			if (licznik == 1)
			{
				tabKod[i] = "1";
				plik2 << i << " " << tabKod[i] << " ";
			}
			else
				plik2 << i << " " << tabKod[i] << " ";
		}
	Kompresja();

	delete[] pixmap;
	plik2.close();
	cout << "Skompresowano obrazek." << endl;
}

void Huffman::Kompresja()
{
	string bufor = "";
	string bufor_pomocniczy = "";
	string bufor_wynikowy = "1";
	int rozmiar = 0;
	int tmp;
	for (long long i = 0; i < PictureInfo.height * PictureInfo.width; ++i)
	{
		rozmiar = 30 - bufor.length();
		if (tabKod[pixmap[i].B].length() == rozmiar)
		{
			bufor += tabKod[pixmap[i].B];
			bufor_wynikowy += bufor;
			tmp = BinToDec(bufor_wynikowy);
			plik2.write((char*)&tmp, 4);
			bufor_wynikowy = "1";
			bufor = "";
		}
		else if (tabKod[pixmap[i].B].length()<rozmiar)
			bufor += tabKod[pixmap[i].B];
		else
		{
			bufor += tabKod[pixmap[i].B];
			bufor_pomocniczy.insert(0, bufor, 30, bufor.length());
			bufor.erase(30, bufor.length());
			bufor_wynikowy += bufor;
			tmp = BinToDec(bufor_wynikowy);
			plik2.write((char*)&tmp, 4);
			bufor_wynikowy = "1";
			bufor = bufor_pomocniczy;
			bufor_pomocniczy = "";
		}
		//-----------------------------------------------------------
		rozmiar = 30 - bufor.length();
		if (tabKod[pixmap[i].G].length() == rozmiar)
		{
			bufor += tabKod[pixmap[i].G];
			bufor_wynikowy += bufor;
			tmp = BinToDec(bufor_wynikowy);
			plik2.write((char*)&tmp, 4);
			bufor_wynikowy = "1";
			bufor = "";
		}
		else if (tabKod[pixmap[i].G].length()<rozmiar)
			bufor += tabKod[pixmap[i].G];
		else
		{
			bufor += tabKod[pixmap[i].G];
			bufor_pomocniczy.insert(0, bufor, 30, bufor.length());
			bufor.erase(30, bufor.length());
			bufor_wynikowy += bufor;
			tmp = BinToDec(bufor_wynikowy);
			plik2.write((char*)&tmp, 4);
			bufor_wynikowy = "1";
			bufor = bufor_pomocniczy;
			bufor_pomocniczy = "";
		}
		//------------------------------------------------------------
		rozmiar = 30 - bufor.length();
		if (tabKod[pixmap[i].R].length() == rozmiar)
		{
			bufor += tabKod[pixmap[i].R];
			bufor_wynikowy += bufor;
			tmp = BinToDec(bufor_wynikowy);
			plik2.write((char*)&tmp, 4);
			bufor_wynikowy = "1";
			bufor = "";
		}
		else if (tabKod[pixmap[i].R].length()<rozmiar)
			bufor += tabKod[pixmap[i].R];
		else
		{
			bufor += tabKod[pixmap[i].R];
			bufor_pomocniczy.insert(0, bufor, 30, bufor.length());
			bufor.erase(30, bufor.length());
			bufor_wynikowy += bufor;
			tmp = BinToDec(bufor_wynikowy);
			plik2.write((char*)&tmp, 4);
			bufor_wynikowy = "1";
			bufor = bufor_pomocniczy;
			bufor_pomocniczy = "";
		}
	}
	if (bufor.length()>0)
	{
		bufor_wynikowy += bufor;
		tmp = BinToDec(bufor_wynikowy);
		plik2.write((char*)&tmp, sizeof(tmp));
	}
}

void Huffman::Dekompresja()
{
    string picture;

    cout << "Podaj nazwe zdekompresowanego obrazka: " << endl;
    cin >> picture;

	OdczytNaglowka("Huffman.xd", plik2);
	ZapisNaglowka(picture+".bmp", plik3);
	short padding = (4 - (PictureInfo.width * 3 % 4)) % 4;
	int licznik;
	plik2 >> licznik;
	tablica *tab = new tablica[licznik];
	for (int i = 0; i<licznik; ++i)
		plik2 >> tab[i].liczba >> tab[i].kod;
	int temp2;
	string temp1;
	string bufor = "";
	int licz = 0;
	temp2 = plik2.get();
	while (!plik2.eof())
	{
		plik2.read((char*)&temp2, 4);
		temp1 = DecToBin((int)temp2);
		for (int k = 0; k<temp1.length(); ++k)
		{
			bufor.insert(bufor.length(), temp1, k, 1);
			for (int i = 0; i<licznik; ++i)
			{
				if (tab[i].kod == bufor)
				{
					plik3 << (char)(tab[i].liczba<<1);
					if (((++licz) % (PictureInfo.width * 3) == 0))
						for (int j = 0; j < padding; j++)
							plik3 << (char)0;
					bufor = "";
					break;
				}
			}
		}
	}
	plik2.close();
	plik3.close();
	cout << "Zdekompresowano obrazek." << endl;
}

//----------------------Funkcje do kompresji----------------------------

string Huffman::DecToBin(int co)
{
	string wynik = "";
	do
	{
		if ((co % 2) == 1)
			wynik = '1' + wynik;
		else
			wynik = '0' + wynik;
		co = co >> 1;
	} while (co != 0);
	wynik.erase(0, 1);
	return wynik;
}

int Huffman::BinToDec(string bin)
{
	int dec = 0;
	int dlugosc = bin.length();
	int i = 0;
	do
	{
		dec += (bin[dlugosc - 1] - 48) * pow(2, i);
		++i;
		--dlugosc;
	} while (dlugosc > 0);
	return dec;
}

//-------------------Funkcje do algorytmu Huffmana-------------------------------------------------------

void Huffman::Zliczanie()
{
	int tab[128]{ 0 };
	for (long long i = 0; i < PictureInfo.height * PictureInfo.width; ++i)
	{
		++tab[pixmap[i].R];
		++tab[pixmap[i].G];
		++tab[pixmap[i].B];
	}
	for (int i = 0; i<128; ++i)
		if (tab[i] != 0)
			Wstawznak(i, tab[i]);
}

void Huffman::Wstawznak(int wartosc, float Hz)
{
	drzewo *nowyznak;
	nowyznak = new drzewo;
	nowyznak->wartosc = wartosc;
	nowyznak->Hz = Hz;
	nowyznak->lewy = nullptr;
	nowyznak->prawy = nullptr;
	kolejkaPriorytetowa.push(*nowyznak);
}

void Huffman::Odczyt(drzewo *odczytana, string s)
{
	if (odczytana->lewy == nullptr)
		tabKod[odczytana->wartosc] = s;
	else
	{
		Odczyt(odczytana->lewy, s + "0");
		Odczyt(odczytana->prawy, s + "1");
	}
}

void Huffman::Korzen(drzewo *&odczytana1, drzewo *&odczytana2)
{
	drzewo *nowyznak;
	nowyznak = new drzewo;
	nowyznak->Hz = odczytana1->Hz + odczytana2->Hz;
	nowyznak->lewy = odczytana1;
	nowyznak->prawy = odczytana2;
	kolejkaPriorytetowa.push(*nowyznak);
}

void Huffman::UsunDrzewo(drzewo *&korzen)
{
	if (korzen->lewy != nullptr)
	{
		UsunDrzewo(korzen->lewy);
	}
	if (korzen->prawy != nullptr)
	{
		UsunDrzewo(korzen->prawy);
	}
	delete korzen;
}
