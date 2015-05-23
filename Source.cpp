#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <time.h>


//*** MAKRA ASEMBLEROWE ***

//pierwszy odczyt czasu, przeniesienie m³odszej czêœci TSC
//do rejestru który nie zostanie nadpisany w drugim etapie
	#define RDTSCP1()	__asm rdtscp	\
		__asm mov ebx, eax

//drugi odczyt czasu, odjêcie od nowej wartoœci TSC wartoœci
//z poprzedniego odczytu i przekazanie uzyskanego wyniku do zmiennej
	#define RDTSCP2(x)	__asm rdtscp	\
		__asm sub eax, ebx	\
		__asm mov x, eax

//wczytanie offsetu tablicy (zmienna "i") do rejestru edi
	#define IterToEDI()	__asm mov edi, dword ptr[i]

//wykonanie operacji dostêpu do komórki pamiêci wskazywanej przez adres pod zmienn¹ "tablica"
//oraz index (offset) wczytany do rejestru edi poprzedni¹ funkcj¹
	#define Odczytaj(tab) __asm mov cl, byte ptr[edi + tab]

//*** FUNKCJE DLA PREPAROWANIA SEKWENCJI ***

//ustawia bie¿¹cy czas podawanemu parametrowi
void startCzas(clock_t *startTime){
	*startTime = clock();
}

//na podstawie aktualnego i przekazanego jako parametr
//wczeœniejszego czasu - oblicza czas, jaki up³yn¹³
double stopCzas(clock_t *startTime){
	return static_cast <double>(clock() - *startTime) / CLOCKS_PER_SEC;
}

//*** ZMIENNE GLOBALNE ***

const unsigned char rozmiarChara = sizeof(char);
const unsigned int rozmiarAlokMega =  450;										//rozmiar w MB (max 1850)
const unsigned int rozmiarAlokByte = rozmiarAlokMega * 1048576 / rozmiarChara;	//konwersja z MB na B
unsigned char tablica[rozmiarAlokByte];											//rezerwacja pamiêci dla programu

const int iloscElemDoOdczytu = 480000000;										//dla techniki preparowania sekwencji
FILE *plik;

//*** MAIN ***
int main(){
	srand((size_t)time(NULL));							//reset generatora liczb pseudolosowych
	DWORD a;											//zmienna przechowuj¹ca obliczon¹ iloœæ cykli CPU


	printf("Trwa inicjalizacja %d MB pamieci losowymi wartosciami (char = %d bajt)...\n", rozmiarAlokMega, rozmiarChara);
	for (int i = 0; i < rozmiarAlokByte; i++)
		tablica[i] = rand() % 256;						//zape³nianie tablicy losowymi wartoœciami z przedzia³u [0-255]

	clock_t *czasStartu = new clock_t;
	double zmierzonyCzas;
	int wybor, odstep = 1;
	printf("Wybierz technike pomiaru:\n1. Preparowanie sekwencji (pomiar masowy)\n2. Pomiar czasu pojedynczej instrukcji\n\nPodaj wybor: ");
	scanf_s("%d", &wybor);
while (true){
	switch (wybor){
	case 1:
		startCzas(czasStartu);
		for (int i = 0; i < iloscElemDoOdczytu; i++){
			IterToEDI();
			Odczytaj(tablica);
		}
		zmierzonyCzas = stopCzas(czasStartu);
		printf("Czas odczytu %d elementow z pamieci wynosi: %.3f [s]\n", iloscElemDoOdczytu, zmierzonyCzas);
		printf("Sredni czas odczytu dla jednej komorki danych wynosi: %.3f [ns]\n", 1000000000*zmierzonyCzas / iloscElemDoOdczytu);
		break;

	case 2:
		system("cls");
		plik = fopen("wyniki.txt", "w");
		printf("Odstepy pomiedzy wczytywanymi danymi: %d bajt (%d char).\n", odstep*rozmiarChara, odstep);
		RDTSCP1();
		RDTSCP2(a);
		printf("Ilosc cykli w przypadku braku instrukcji (nop) = %d\n\n", a);

		for (int i = 0; i < 2050 * odstep; i += odstep) {	//wykonanie 70 prób na kolejnych elementach tablicy
			IterToEDI();
			RDTSCP1();									//czas start
			Odczytaj(tablica);							//wycztanie chara spod adresu "tablica" + "i" do rejestru cl
			RDTSCP2(a);									//czas stop
			printf("%.02d. odczyt = %d\n", i / odstep, a);
			fprintf(plik, "%.02d. odczyt = %d\n", i / odstep, a);
		}
		fclose(plik);
	}
	if (13 == _getch())									//czeka na znak z klawiatury
		odstep *= 2;									//jeœli enter, to podwaja odstêp
}
	return 0;
}