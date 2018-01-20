#include <iostream>
#include <cstdlib>
#include "ObslugaPliku.h"
#include "Huffman.h"
#include "ByteRun.h"
#include "BitComp.h"

using namespace std;

int main()
{
    int  option;

    cout << "Witam w programie kompresji plikow bmp !" << endl;
    do
    {


    cout << "Dostepne opcje: " << endl
    << "1. Kompresja Huffman'a" << endl
    << "2. Kompresja Huffman'a w skali szaroœci" << endl
    << "3. Kompresja ByteRun" << endl
    << "4. Kompresja ByteRun w skali szaroœci" << endl
    << "5. Kompresja z 8 bitow na 7 bitow" << endl
    << "6. Kompresja z 8 bitow na 7 bitow w skali szarosci" << endl
    << "7. Wyjscie z programu" << endl;

    do
    {
       cin >> option;

       if(option > 7 || option < 1)
       {
           cout << "Podano cyfre spoza zakresu ! Wybierz ponownie: " << endl;
       }

    }while(option > 7 || option < 1);

    switch(option)
    {
        case 1:
            {
                Huffman H;
                H.OdczytBMP(false);
                H.Zliczanie();
                H.AlgorytmHuffmana();
                H.NaszFormat();
                H.Dekompresja();
                break;
            }
        case 2:
            {
                Huffman H;
                H.OdczytBMP(true);
                H.Zliczanie();
                H.AlgorytmHuffmana();
                H.NaszFormat();
                H.Dekompresja();
                break;
            }

        case 3:
            {
                ByteRun B;
                B.ByteRunStart();
                break;
            }
        case 4:
            {
                ByteRun B;
                B.ma_byc_czarnobialy = 1;
                B.ByteRunStart();
                break;
            }
        case 5:
            {
                BitComp BC;
                BC.BitCompStart();
                break;
            }
        case 6:
            {
                BitComp BC;
                BC.ma_byc_czarnobialy = 1;
                BC.BitCompStart();
                break;
            }

        case 7: cout << "Program zkonczyl dzialanie." << endl; exit(0);  break;
    }

    cout << "Zakonczono wybrana opcje." << endl << endl;
    }while(option);
    return 0;
}
