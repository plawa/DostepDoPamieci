OPIS PROGRAMU
-----------------
Program s�u�y do mierzenia ilo�ci cykli procesora zliczonych podczas podstawowych operacji dost�pu do pami�ci.
Szczeg�owy opis zawarty jest w sprawozdaniu.

URUCHAMIANIE
-----------------
W celu przyst�pienia do uruchomienia aplikacji, nale�y otworzy� projekt zapisany w pliku o rozszerzeniu .vcxproj, po czym skompilowa� go.
�rodowisko programistyczne, w jakim program by� rozwijany to Visual Studio 2013, tote� do kompilacji zalecane jest u�ywanie w�a�nie tego narz�dzia.
Aby uruchomi� aplikacj�, nale�y nast�pnie otworzy� plik wykonywalny (*.exe), kt�ry znajduje si� w folderze "/debug".

OBS�UGA
-----------------
Ze wzgl�du na udost�pniony kod �r�d�owy, przed kompilacj� mo�liwa jest edycja dowolnych parametr�w maj�cych wp�yw na przebieg pomiar�w. Parametry maj�ce najwi�kszy wp�yw na wyniki pomiar�w to wielko�� skoku, oraz rozmiar zajmowanego przez program w pami�ci miejsca. Znajduj� si� one przed funkcj� main(), w sekcji kodu okre�lonej komentarzem �***ZMIENNE MODYFIKOWALNE***�.
Po uzyskaniu pierwszej serii wynik�w, naci�ni�cie dowolnego klawisza ponawia test, a przycisk "Enter" - zwi�ksza skok odczytu kolejnych element�w.