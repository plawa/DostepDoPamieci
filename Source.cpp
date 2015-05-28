#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <time.h>

//*** MAKRA ASEMBLEROWE ***
//pierwszy odczyt czasu, przeniesienie m�odszej cz�ci TSC
//do rejestru kt�ry nie zostanie nadpisany w drugim etapie
	#define RDTSCP1()	__asm rdtscp	\
		__asm mov ebx, eax

//drugi odczyt czasu, odj�cie od nowej warto�ci TSC warto�ci
//z poprzedniego odczytu i przekazanie uzyskanego wyniku do zmiennej
	#define RDTSCP2(x)	__asm rdtscp	\
		__asm sub eax, ebx	\
		__asm mov x, eax

//wczytanie offsetu tablicy (zmienna "i") do rejestru edi
	#define IterToEDI()	__asm mov edi, dword ptr[i]

//wykonanie operacji dost�pu do kom�rki pami�ci wskazywanej przez adres pod zmienn� "tablica"
//oraz index (offset) wczytany do rejestru edi poprzedni� funkcj�
	#define Odczytaj(tab) __asm mov cl, byte ptr[edi + tab]


//*** FUNKCJE DLA PREPAROWANIA SEKWENCJI ***
//ustawia bie��cy czas podawanemu parametrowi
void startCzas(clock_t *startTime){
	*startTime = clock();
}

//na podstawie aktualnego i przekazanego jako parametr
//wcze�niejszego czasu - oblicza i zwraca czas, jaki up�yn��
double stopCzas(clock_t *startTime){
	return static_cast <double>(clock() - *startTime) / CLOCKS_PER_SEC;
}

//*** ZMIENNE GLOBALNE ***
const unsigned char rozmiarChara = sizeof(char);
const unsigned int rozmiarAlokMega =  480;										//rozmiar w MB (max 1850)
const unsigned int rozmiarAlokByte = rozmiarAlokMega * 1048576 / rozmiarChara;	//konwersja z MB na B
unsigned char tablica[rozmiarAlokByte];											//rezerwacja pami�ci dla programu

const int iloscElemDoOdczytu = 480000000;										//dla techniki preparowania sekwencji
FILE *plik;

//*** MAIN ***
int main(){
	srand((size_t)time(NULL));							//reset generatora liczb pseudolosowych
	unsigned int timer, wybor, odstep = 1;					//zmienna przechowuj�ca obliczon� ilo�� cykli CPU
	double zmierzonyCzas;
	clock_t *czasStartu = new clock_t;

	printf("Trwa inicjalizacja %d MB pamieci losowymi wartosciami (char = %d bajt)...\n", rozmiarAlokMega, rozmiarChara);
	for (int i = 0; i < rozmiarAlokByte; i++)
		tablica[i] = rand() % 256;						//zape�nianie tablicy losowymi warto�ciami z przedzia�u [0-255]
	
	printf("Wybierz technike pomiaru:\n1. Preparowanie sekwencji (pomiar masowy)\n2. Pomiar czasu pojedynczej instrukcji\n\nPodaj wybor: ");
	scanf_s("%d", &wybor);

while (true){
	switch (wybor){
	case 1:
		unsigned int skok;
		printf("Podaj odstep miedzy komorkami: ");
		scanf_s("%d", &skok);
		startCzas(czasStartu);
		for (int i = 0; i < iloscElemDoOdczytu; i+= skok){
			IterToEDI();
			Odczytaj(tablica);
		}
		zmierzonyCzas = stopCzas(czasStartu);
		printf("Czas odczytu %d elementow z pamieci wynosi: %.3f [s]\n", iloscElemDoOdczytu/skok, zmierzonyCzas);
		printf("Sredni czas odczytu dla jednej komorki danych wynosi: %.3f [ns]\n", 1000000000 * zmierzonyCzas/(iloscElemDoOdczytu/skok));
		break;

	case 2:
		system("cls");
		plik = fopen("wyniki.txt", "w");
		printf("Odstepy pomiedzy wczytywanymi danymi: %d bajt (%d char).\n", odstep*rozmiarChara, odstep);
		RDTSCP1();
		RDTSCP2(timer);
		printf("Ilosc cykli w przypadku braku instrukcji (nop) = %d\n\n", timer);

		for (unsigned int i = 0; i < 2050; i += odstep) {
			IterToEDI();								//wczytanie offsetu "i" do rej. %edi, aby nie mierzy� kilkukrotnego dost�pu do pamieci
			RDTSCP1();									//czas start
			Odczytaj(tablica);							//wycztanie chara spod adresu "tablica" + "%edi" do rejestru cl
			RDTSCP2(timer);								//czas stop
			fprintf(plik, "%.02d. odczyt = %d\n", i / odstep, timer); //zapis linii do pliku
		}
		printf("Zakonczono. Wyniki w pliku \"wyniki.txt\".\n");
		fclose(plik);
	}
	if (13 == _getch())									//czeka na znak z klawiatury
		odstep *= 2;									//je�li enter, to podwaja odst�p mi�dzy kom�rkami
}
	return 0;
}