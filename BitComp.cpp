#include "BitComp.h"
#include "ObslugaPliku.h"
#include <vector>
#include <bitset>

BitComp::BitComp()
{
	ma_byc_czarnobialy = 0;
}

void BitComp::BitCompStart()
{
    vector <signed char> bufwyj;

    WczytajBitmape(bufwyj); // wczytujemy bitmape do wektora bez pustych pikseli
	BitCompKompresuj(bufwyj);
	BitCompDekompresuj();
}

void BitComp::WczytajBitmape(vector <signed char> &bufwyj)
{
    string picture;

    cout << "Podaj nazwe obrazku jaki chcesz skompresowac: " << endl;
    cin >> picture;

    OdczytNaglowka(picture+".bmp", plik);

    int row_padding = (4-((PictureInfo.width*3)%4))%4;
    char one_byte;  // zmienna do odczytu danych o wielkosci 1 bajtu
    int temp = 0;

	if (ma_byc_czarnobialy){
        for(int i = 0; i < PictureInfo.height; i++){
            for(int j = 0; j < PictureInfo.width; j++){

                temp = 0.299 * plik.get() + 0.587 * plik.get() + 0.114 * plik.get();

                bufwyj.push_back(temp);
                bufwyj.push_back(temp);
                bufwyj.push_back(temp);
            }
        plik.seekg(row_padding, std::ios::cur); //pomijamy puste bajty uzupelniajace kazdy wiersz
        }

    } else{
        for(int i = 0; i < PictureInfo.height; i++){
            for(int j = 0; j < PictureInfo.width*3; j++){
                plik.get(one_byte);
                bufwyj.push_back(one_byte);
            }
            plik.seekg(row_padding, std::ios::cur); //pomijamy puste bajty uzupelniajace kazdy wiersz
        }
    }
    plik.close();
}

void BitComp::BitCompKompresuj(vector <signed char> &bufwyj)
{
	vector <signed char> temp;

    char c, bajt1, bajt2;
    int przesuniecie = 0;
    int maski[8] = {0b11111111, 0b01111111, 0b00111111, 0b00011111, 0b00001111,0b00000111,0b00000011,0b00000001};

	for(int i=0;i<bufwyj.size();i++)
    {
        c = 0;
        bajt1 = bufwyj[i];
        bajt1 = bajt1 & 0b11111110;
        bajt1 = bajt1 << przesuniecie;

        bajt2 = bufwyj[i+1];
        bajt2 = bajt2 & 0b11111110; //obcinamy do 7 bitow
        bajt2 = bajt2 >> (7 - przesuniecie);
        bajt2 = bajt2 & maski[7 - przesuniecie];

        c = bajt1 | bajt2;
        temp.push_back(c);

        przesuniecie++;
        if(przesuniecie > 7) przesuniecie = 0;
        if(przesuniecie == 0) i++;
    }

	ZapisBitComp(plik, temp);


}

void BitComp::BitCompDekompresuj()
{
	OdczytNaglowka("BitComp.xd", plik2);

	vector <signed char> file_vec;
	vector <signed char> bufwej;

    char c, bajt1, bajt2;
    int przesuniecie = 0;
    int maski[8] = {0b11111111, 0b01111111, 0b00111111, 0b00011111, 0b00001111,0b00000111,0b00000011,0b00000001};

    char one_byte;

    while(plik2.get(one_byte))
        file_vec.push_back(one_byte);


    for(int i=0;i<file_vec.size();i++)
    {
        if(przesuniecie == 0)
        {
            c = 0;
            bajt1 = file_vec[i];
            bajt1 = bajt1 >> przesuniecie;
            c = bajt1 & maski[przesuniecie];
            bufwej.push_back(c);
            przesuniecie++;
            i--;
        }
        else
        {
            c = 0;
            bajt1 = file_vec[i];
            bajt1 = bajt1 << (8 - przesuniecie);

            bajt2 = file_vec[i+1];
            bajt2 = bajt2 >> przesuniecie;
            bajt2 = bajt2 & maski[przesuniecie];

            c = bajt1 | bajt2;
            c = c & 0b11111110;
            bufwej.push_back(c);

            przesuniecie++;
            if(przesuniecie > 8) przesuniecie = 0;
        }
    }

    plik2.close();
	BitComp_ZapisDoBMP(plik2, bufwej);

}

void BitComp::BitComp_ZapisDoBMP(fstream &plik3, vector <signed char> &bufwej) {

    string picture;

    cout << "Podaj nazwe zdekompresowanego obrazka: " << endl;
    cin >> picture;

	ZapisNaglowka(picture+".bmp", plik3);

	plik3.close();

	long long size = PictureInfo.height * PictureInfo.width * 3;
	short padding = (4 - (PictureInfo.width * 3 % 4)) % 4;
	int licz = 0;

	plik3.open(picture+".bmp", ios::app | ios::binary);
	if (plik3) {
		for (long long i = 0; i < size; ++i) {
			plik3 << bufwej[i];
			// jesli zapisano ostatni bajt w linii, dodaj tyle zer, ile wynosi padding
			// liczba bajtow w linii - 3*szerokosc
			if (((++licz) % (PictureInfo.width * 3) == 0)) {
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

void BitComp::ZapisBitComp(fstream &plik2, vector <signed char> &bufwyj) {
	ZapisNaglowka("BitComp.xd", plik2);
	plik2.close();
	plik2.open("BitComp.xd", ios::app | ios::binary);

	if (plik2) {
		for (long long i = 0; i<bufwyj.size(); ++i) {

			plik2 << bufwyj[i];
		}
	}
	else {

		cout << "Nie mozna dopisac do BitComp.xd\n";
	}
	plik2.close();
	cout << "Skompresowano obrazek." << endl;
}
