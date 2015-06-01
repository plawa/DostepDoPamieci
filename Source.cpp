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
//wczeœniejszego czasu - oblicza i zwraca czas, jaki up³yn¹³
double stopCzas(clock_t *startTime){
	return static_cast <double>(clock() - *startTime) / CLOCKS_PER_SEC;
}

//*** ZMIENNE GLOBALNE ***
unsigned int odstep = 1;														//odstêp - wielkoœæ skoku pomiêdzy komórkami danych
const unsigned int rozmiarAlokMega =  480;										//rozmiar zajmowanego przez program miejsca w pamiêci w MB

//------------------------------------------
const unsigned char rozmiarChara = sizeof(char);
const unsigned int rozmiarAlokByte = rozmiarAlokMega * 1048576 / rozmiarChara;	//konwersja z MB na B
unsigned char tablica[rozmiarAlokByte];											//rezerwacja pamiêci dla programu
unsigned char tablica2[rozmiarAlokByte];											//rezerwacja pamiêci dla programu - druga tablica
unsigned char tablica3[rozmiarAlokByte];
FILE *plik;

//*** MAIN ***
int main(){
	srand((size_t)time(NULL));							//reset generatora liczb pseudolosowych
	unsigned int timer, wybor;
	double zmierzonyCzas;
	clock_t *czasStartu = new clock_t;

	printf("Trwa inicjalizacja %d MB pamieci losowymi wartosciami (char = %d bajt)...\n", rozmiarAlokMega, rozmiarChara);
	for (int i = 0; i < rozmiarAlokByte; i++)
		tablica3[i] = tablica2[i] = tablica[i] = rand() % 256;						//zape³nianie tablicy losowymi wartoœciami z przedzia³u [0-255]
	
	printf("Wybierz technike pomiaru:\n1. Preparowanie sekwencji (pomiar masowy)\n2. Pomiar czasu pojedynczej instrukcji\n\nPodaj wybor: ");
	scanf_s("%d", &wybor);

while (true){
	switch (wybor){
	case 1:
		unsigned int skok;
		printf("Podaj odstep miedzy komorkami: ");
		scanf_s("%d", &skok);
		startCzas(czasStartu);
		for (unsigned int i = 0; i < rozmiarAlokByte; i+= skok){
			IterToEDI();
			Odczytaj(tablica);
			Odczytaj(tablica2);
		}
		zmierzonyCzas = stopCzas(czasStartu);
		printf("Czas odczytu %d elementow z pamieci wynosi: %.3f [s]\n", 2*rozmiarAlokByte/skok, zmierzonyCzas);
		printf("Sredni czas odczytu dla jednej komorki danych wynosi: %.3f [ns]\n", 1000000000 * zmierzonyCzas/(2*rozmiarAlokByte/skok));
		break;

	case 2:
		system("cls");
		plik = fopen("wyniki.txt", "w");
		printf("Odstepy pomiedzy wczytywanymi danymi: %d bajt (%d char).\n", odstep*rozmiarChara, odstep);
		RDTSCP1();
		RDTSCP2(timer);
		printf("Ilosc cykli w przypadku braku instrukcji (nop) = %d\n\n", timer);

		for (unsigned int i = 0; i < 2050; i += odstep) {
			IterToEDI();								//wczytanie offsetu "i" do rej. %edi, aby nie mierzyæ kilkukrotnego dostêpu do pamieci
			RDTSCP1();									//czas start
			Odczytaj(tablica);							//wycztanie chara spod adresu "tablica" + "%edi" do rejestru cl
			RDTSCP2(timer);								//czas stop
			fprintf(plik, "%.02d. odczyt = %d\n", i / odstep, timer); //zapis linii do pliku
		}
		printf("Zakonczono. Wyniki w pliku \"wyniki.txt\".\n");
		fclose(plik);
	}
	if (13 == _getch())									//czeka na znak z klawiatury
		odstep *= 2;									//jeœli enter, to podwaja odstêp miêdzy komórkami
}
	return 0;
}