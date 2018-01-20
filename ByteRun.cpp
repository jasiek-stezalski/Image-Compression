#include "ByteRun.h"
#include "ObslugaPliku.h"
#include <vector>




ByteRun::ByteRun()
{
	ma_byc_czarnobialy = 0;
	padding = 0;
}

void ByteRun::ByteRunStart() {

	ByteRunKompresuj();
	ByteRunDekompresuj();

}

void ByteRun::ByteRunKompresuj()
{
	//****************** BMP -> TMP ***********************************//

	string picture;

    cout << "Podaj nazwe obrazku jaki chcesz skompresowac: " << endl;
    cin >> picture;

	OdczytNaglowka(picture+".bmp", plik);

	//Wczytanie poszczególnych składowych kolorów do tablicy struktur RGB
	RGB* pixele = nullptr;
	WczytajBitmapeDoBufora(plik, pixele);

	if (ma_byc_czarnobialy) ColorToGray(pixele);



	// Wywolanie algorytmu byterun
	vector <signed char> bufwyj;						// bufor przychowujący skompresowaną bitmapę
	ByteRunCompress(pixele, bufwyj);

	//Zapis do pliku "ByteRun.tmp"
	ZapisByteRun(plik2, bufwyj);
	//****************** BMP -> TMP ***********************************//
}

void ByteRun::ByteRunDekompresuj()
{
	//****************** TMP -> BMP ***********************************//

	//Odczyt z pliku "ByteRun.tmp"
	OdczytNaglowka("ByteRun.xd", plik2);
	vector <char> bufwej;
	//char *bufwej = nullptr;
	long long dlugosc_skompresowane = Odczytztmp(plik2, bufwej);


	unsigned char* bufdecompressed = nullptr;		// bufor dekompresji



													// bufor odczytu z tmp

													// Dekompresja i zapis do bufora (bufdecompressed)
	ByteRunDecompress(bufwej, bufdecompressed, dlugosc_skompresowane);
	// Zapis zawartości bufora do pliku .bmp
	ByteRun_ZapisDoBMP(plik3, bufdecompressed);

}

void ByteRun::ColorToGray(RGB *&pixele) {
	long long pixels = PictureInfo.height * PictureInfo.width;

	int temp;
	for (long long i = 0; i < pixels; ++i) {
		temp = 0.299 * pixele[i].R + 0.587 * pixele[i].G + 0.114 * pixele[i].B;
		pixele[i].B = pixele[i].G = pixele[i].R = temp;

	}

}


void ByteRun::WczytajBitmapeDoBufora(fstream &plik, RGB *&pixele)
{

	long long pixels = PictureInfo.height * PictureInfo.width;
	TworzenieTablicy(pixele, pixels);

	short padding = (4 - ((PictureInfo.width * 3) % 4)) % 4;

	long long i = 0;
	while (i < pixels) {


		if (plik) {
			pixele[i].B = plik.get();
			pixele[i].G = plik.get();
			pixele[i].R = plik.get();

			pixele[i].B >>= 1;
			pixele[i].G >>= 1;
			pixele[i].R >>= 1;
		}
		else {
			cout << "Blad wczytywania bitmapy do bufora \n";
		}
		// jeśli dotarto do końca wiersza, omiń dodatkowe zera, wyrównujące do 4 pixeli
		if ((i + 1) % PictureInfo.width == 0) {
			plik.ignore(padding);
		}
		++i;
	}

	plik.close();


}
// zwraca dlugosc bufora po skompresowaniu
void ByteRun::ByteRunCompress(RGB *pixele, vector<signed char> &bufwyj) {
	long long pixels = PictureInfo.height * PictureInfo.width;
	long long i = 0;													// indeks aktualnie porownywanego koloru
	short j = 0;	 													// licznik dlugosci sekwencji (roznych kolorow lub powtorzen) max 127

																		// jesli obrazek kolorowy

																		// dopóki sa piksele do skompresowania
	while (i <= pixels - 1) {
		// jesli zaczyna sie sekwencja powtorzen
		if (czyTenSamKolor(pixele, i))	// porownuje pixel[i] i pixel[i+1];
		{
			//zmierz dlugosc sekwencji
			j = 0;
			while ((i + j < pixels) &&					// jesli jestes w zakresie tablicy
				(czyTenSamKolor(pixele, i + j)) &&			// i sasiednie pixele maja taki sam kolor
				(j < 127))									// i licznik powtorzen miesci sie w zakresie chara
			{
				++j;										// inkrementuj licznik
			}

			// zapisz do bufora wyjsciowego

			bufwyj.push_back(-j);
			bufwyj.push_back(pixele[i + j].B);
			bufwyj.push_back(pixele[i + j].G);
			bufwyj.push_back(pixele[i + j].R);
			//przesun wskaznik o dlugosc sekwencji
			i += (j + 1);
		}
		// jesli zaczyna sie sekwencja bez powtorzen
		else
		{
			//zmierz dlugosc sekwencji
			j = 0;
			while ((i + j < pixels) &&						// jesli jestes w zakresie tablicy
				(!czyTenSamKolor(pixele, i + j)) &&				// i sasiednie pixele maja rozny kolor
				(j < 128))										// i licznik w zakresie chara
			{
				++j;											// inkrementuj licznik
			}

			//wypisz spakowana sekwencje

			bufwyj.push_back(j - 1);


			for (int z = 0; z < j; z++)
			{

				bufwyj.push_back(pixele[i + z].B);
				bufwyj.push_back(pixele[i + z].G);
				bufwyj.push_back(pixele[i + z].R);

			}
			//przesun wskaznik o dlugosc sekwencji
			i += j;
		}
	}



	delete[] pixele;
}

void ByteRun::ByteRun_ZapisDoBMP(fstream &plik3, unsigned char* bufdecompressed) {
    string picture;

    cout << "Podaj nazwe zdekompresowanego obrazka: " << endl;
    cin >> picture;
	ZapisNaglowka(picture+".bmp", plik3);
	plik3.close();		//obejscie zamykania pliku 2
						//int size = FileInfo.fileSize - FileInfo.dataOffset;


	long long size = PictureInfo.height * PictureInfo.width * 3;
	short padding = (4 - (PictureInfo.width * 3 % 4)) % 4;

	plik3.open(picture+".bmp", ios::app | ios::binary);
	if (plik3) {
		for (long long i = 0; i < size; ++i) {
			plik3 << bufdecompressed[i];
			// jesli zapisano ostatni bajt w linii, dodaj tyle zer, ile wynosi padding
			// liczba bajtow w linii - 3*szerokosc
			if (((i + 1) % (PictureInfo.width * 3) == 0)) {
				for (int j = 0; j < padding; j++) plik3 << (char)0;

			}

		}
	}
	else {
		cout << "Nie udalo sie zapisac do .bmp";
	}


	plik3.close();

	cout << "Zdekompresowano obrazek." << endl;
}

void ByteRun::ByteRunDecompress(vector <char> &bufwej, unsigned char *&bufdecompressed, long long dlugosc) {
	//TworzenieTablicy(bufdecompressed, FileInfo.fileSize - FileInfo.dataOffset);
	TworzenieTablicy(bufdecompressed, PictureInfo.height * PictureInfo.width * 3);
	long long i = 0;		//indeks liczby powtorzen z bufora skompresowanego
	long long k = 0;		//indeks bufora zdekompresowanego


	while (i < dlugosc) {
		// sekwencja powtarzajacych sie bajtow
		if (bufwej[i] < 0) {
			// wypisz -n+1 razy
			for (short j = 0; j < -(bufwej[i]) + 1; ++j) {

				bufdecompressed[k] = bufwej[i + 1]; bufdecompressed[k++] <<= 1;
				bufdecompressed[k] = bufwej[i + 2]; bufdecompressed[k++] <<= 1;
				bufdecompressed[k] = bufwej[i + 3]; bufdecompressed[k++] <<= 1;

			}
			i += 4;
		}
		// sekwencja roznych bajtow
		else {
			for (short j = 0; j < bufwej[i] + 1; ++j) {

				bufdecompressed[k] = bufwej[i + 1 + j * 3];	 bufdecompressed[k++] <<= 1;
				bufdecompressed[k] = bufwej[i + 2 + j * 3];	 bufdecompressed[k++] <<= 1;
				bufdecompressed[k] = bufwej[i + 3 + j * 3];	 bufdecompressed[k++] <<= 1;
			}
			// i - indeks elementu trzymajacego informacje o liczbie powtorzen (lub ciagu roznych kolorow)
			// nalezy go przesunac o:
			//(bufwej[i] + 1) - tyle zestawow trojek bajtow sie nie powtarza
			//*3 bo 3 bajty na kolor; +1 - nastepny bajt
			i += (bufwej[i] + 1) * 3 + 1;

		}
	}

	//delete[] bufwej;
}

long long ByteRun::Odczytztmp(fstream &plik2, vector <char> & bufwej) {
	//	char* bufwej = new char[FileInfo.fileSize - FileInfo.dataOffset]{0};
	// TODO: zmienic na wektor?
	//TworzenieTablicy(bufwej, PictureInfo.height * PictureInfo.width * 4); //Najgorszy przypadek - po skompresowaniu rozmiar = liczba pixeli * 3  + bajt powtorzen co 127(B,G,R + powtorzenia)
	long long i = 0;
	while (!plik2.eof()) {
		bufwej.push_back(plik2.get());
		i++;

	}
	return i - 1;
}
void ByteRun::ZapisByteRun(fstream &plik2, vector <signed char> &bufwyj) {
	ZapisNaglowka("ByteRun.xd", plik2);
	plik2.close();							// obejscie tego, ze nie mozna zamknac pliku w ZapisNaglowka, bo posypie sie Huffman
	plik2.open("ByteRun.xd", ios::app | ios::binary);

	if (plik2) {
		for (long long i = 0; i<bufwyj.size(); ++i) {

			plik2 << bufwyj[i];
		}
	}
	else {

		cout << "Nie mozna dopisac do ByteRun.tmp\n";
	}
	plik2.close();
	cout << "Skompresowano obrazek." << endl;
	//delete[]bufwyj;


}


bool ByteRun::czyTenSamKolor(RGB *pixele, int index) {
	if (pixele[index].R == pixele[index + 1].R &&
		pixele[index].G == pixele[index + 1].G &&
		pixele[index].B == pixele[index + 1].B)
		return true;
	else return false;
}


/*
void ByteRun::TworzenieTablicy(RGB *&tab, long long a)
{
	try
	{
		tab = new RGB[a]{ 0 };
	}
	catch (bad_alloc)
	{
		cout << "Brak miejsca na utworzenie tablicy RGB";
		delete[] tab;
		cin.get();
		exit(0);
	}
}

void ByteRun::TworzenieTablicy(char *&tab, long long a)
{
	try
	{
		tab = new char[a] { 0 };
	}
	catch (bad_alloc)
	{
		cout << "Brak miejsca na utworzenie tablicy char";
		delete[] tab;
		cin.get();
		exit(0);
	}
}

void ByteRun::TworzenieTablicy(unsigned char *&tab, long long a)
{
	try
	{
		tab = new unsigned char[a] { 0 };
	}
	catch (bad_alloc)
	{
		cout << "Brak miejsca na utworzenie tablicy unsigned char";
		delete[] tab;
		cin.get();
		exit(0);
	}
}*/
