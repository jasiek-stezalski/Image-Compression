#include <iostream>
#include <fstream>

#ifndef OBSLUGAPLIKU_H
#define OBSLUGAPLIKU_H

using namespace std;

class ObslugaPliku
{
public:

	typedef struct FILEHEADER
	{
		short type;
		int fileSize;
		short reserved0;
		short reserved1;
		int dataOffset;
	}FILEHEADER;

	typedef struct INFOHEADER
	{
		int hdrSize;
		int width;
		short planes;
		int height;
		short depth;
		int compression;
		int bmpDataSize;
		int hResolution;
		int vResolution;
		int nColors;
		int nImportantColors;
	}INFOHEADER;

	typedef struct RGB
	{
		unsigned char R;
		unsigned char G;
		unsigned char B;
	}Pixel;

	Pixel* pixmap;
	FILEHEADER FileInfo;
	INFOHEADER PictureInfo;

	// Funkcje do obslugi plikow
	void OdczytNaglowka(string nazwaPliku, fstream &plik);
	void ZapisNaglowka(string nazwaPliku, fstream &plik);
	void OdczytBitmapy(fstream &plik, bool szary);
	void TablicaPixeli();
};

#endif
