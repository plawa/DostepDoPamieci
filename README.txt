OPIS PROGRAMU
-----------------
Program s³u¿y do mierzenia iloœci cykli procesora zliczonych podczas podstawowych operacji dostêpu do pamiêci.
Szczegó³owy opis zawarty jest w pliku "Sprawozdanie z projektu.pdf".

URUCHAMIANIE
-----------------
W celu przyst¹pienia do uruchomienia aplikacji, nale¿y otworzyæ projekt zapisany w pliku o rozszerzeniu .vcxproj, po czym skompilowaæ go.
Œrodowisko programistyczne, w jakim program by³ rozwijany to Visual Studio 2013, tote¿ do kompilacji zalecane jest u¿ywanie w³aœnie tego narzêdzia.
Aby uruchomiæ aplikacjê, nale¿y po jej kompilacji otworzyæ plik wykonywalny (*.exe), który znajduje siê w folderze "/debug".

OBS£UGA
-----------------
Ze wzglêdu na udostêpniony kod Ÿród³owy, przed kompilacj¹ mo¿liwa jest edycja dowolnych parametrów maj¹cych wp³yw na przebieg pomiarów. Parametry maj¹ce najwiêkszy wp³yw na wyniki pomiarów to wielkoœæ skoku, oraz rozmiar zajmowanego przez program w pamiêci miejsca. Znajduj¹ siê one przed funkcj¹ main(), w sekcji kodu okreœlonej komentarzem „*** ZMIENNE MODYFIKOWALNE ***”.
Dok³adne wytyczne co nale¿y zrobiæ aby ponowiæ eksperymenty opisane w sprawozdaniu znajduj¹ siê w dokumencie "Instrukcje pomiarów.pdf".