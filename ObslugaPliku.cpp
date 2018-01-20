#include "ObslugaPliku.h"
#include <stdlib.h>

void ObslugaPliku::OdczytNaglowka(string nazwaPliku, fstream &plik)
{
	plik.open(nazwaPliku, ios::in | ios::binary);
	if (plik.good())
	{
		plik.read((char*)&FileInfo.type, 2);
		plik.read((char*)&FileInfo.fileSize, 4);
		plik.read((char*)&FileInfo.reserved0, 2);
		plik.read((char*)&FileInfo.reserved1, 2);
		plik.read((char*)&FileInfo.dataOffset, 4);
		plik.read((char*)&PictureInfo.hdrSize, 4);
		plik.read((char*)&PictureInfo.width, 4);
		plik.read((char*)&PictureInfo.height, 4);
		plik.read((char*)&PictureInfo.planes, 2);
		plik.read((char*)&PictureInfo.depth, 2);
		plik.read((char*)&PictureInfo.compression, 4);
		plik.read((char*)&PictureInfo.bmpDataSize, 4);
		plik.read((char*)&PictureInfo.hResolution, 4);
		plik.read((char*)&PictureInfo.vResolution, 4);
		plik.read((char*)&PictureInfo.nColors, 4);
		plik.read((char*)&PictureInfo.nImportantColors, 4);
	}
	else
	{
		cout << "Blad odczytu pliku!";
		cin.get();
		exit(0);
	}
}

void ObslugaPliku::ZapisNaglowka(string nazwaPliku, fstream &plik)
{
	plik.open(nazwaPliku, ios::out | ios::binary);
	if (plik.good())
	{
		plik.write((char*)&FileInfo.type, 2);
		plik.write((char*)&FileInfo.fileSize, 4);
		plik.write((char*)&FileInfo.reserved0, 2);
		plik.write((char*)&FileInfo.reserved1, 2);
		plik.write((char*)&FileInfo.dataOffset, 4);
		plik.write((char*)&PictureInfo.hdrSize, 4);
		plik.write((char*)&PictureInfo.width, 4);
		plik.write((char*)&PictureInfo.height, 4);
		plik.write((char*)&PictureInfo.planes, 2);
		plik.write((char*)&PictureInfo.depth, 2);
		plik.write((char*)&PictureInfo.compression, 4);
		plik.write((char*)&PictureInfo.bmpDataSize, 4);
		plik.write((char*)&PictureInfo.hResolution, 4);
		plik.write((char*)&PictureInfo.vResolution, 4);
		plik.write((char*)&PictureInfo.nColors, 4);
		plik.write((char*)&PictureInfo.nImportantColors, 4);
	}
	else
	{
		cout << "Blad odczytu pliku!";
		cin.get();
		exit(0);
	}
}

void ObslugaPliku::TablicaPixeli()
{
	try
	{
		pixmap = new Pixel[PictureInfo.height * PictureInfo.width];
	}
	catch (bad_alloc)
	{
		cout << "Brak miejsca na utworzenie tablicy";
		cin.get();
		exit(0);
	}
	for (long long i = 0; i < PictureInfo.height * PictureInfo.width; ++i)
	{
		pixmap[i].R = 0;
		pixmap[i].G = 0;
		pixmap[i].B = 0;
	}
}

void ObslugaPliku::OdczytBitmapy(fstream &plik, bool szary)
{
	short padding = (4 - (PictureInfo.width * 3 % 4)) % 4;
	if (szary)
	{
		int temp = 0;
		for (long long i = 0; i < PictureInfo.height * PictureInfo.width; ++i)
		{
			temp = 0.299 * plik.get() + 0.587 * plik.get() + 0.114 * plik.get();
			temp = temp>>1;
			pixmap[i].B = temp;
			pixmap[i].G = temp;
			pixmap[i].R = temp;
			if ((i + 1) % PictureInfo.width == 0) {
				plik.ignore(padding);
			}
		}
	}
	else
	{
		for (long long i = 0; i < PictureInfo.height * PictureInfo.width; ++i)
		{
			pixmap[i].B = plik.get()>>1;
			pixmap[i].G = plik.get()>>1;
			pixmap[i].R = plik.get()>>1;
			if ((i + 1) % PictureInfo.width == 0) {
				plik.ignore(padding);
			}
		}
	}
}


