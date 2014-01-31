Prosty, emulowany system plików
===============================


Informacje ogólne
-----------------

*Prosty, emulowany system plików* (dalej *SEFS*) to, jak nazwa wskazuje,
system plików zaimplementowany jako prosta biblioteka w języku C. *Emulowany*
ponieważ pisanie systemu na poziomie modułu sterującego dysku twardego jest
zbyt karkołomnym zadaniem. Zamiast tego, system plików jest zamontowany w pliku
a czytanie, pisanie i przesuwanie "głowicy" odbywa się odpowiednio za pomocą
funkcji `fread`, `fwrite` i `fseek`.

Oczywiście całość powstała tylko i wyłącznie w celach edukacyjnych i jest
kompletnie nieefektywna. Choć całkiem praktycznym zastosowaniem biblioteki może
być na przykład archiwizacja plików.


Sposób implementacji
--------------------

Jako metoda przydziału miejsca na dysku został wybrany przydział indeksowy, zaś
do reprezentacji wolnej przestrzeni wektor bitowy. Istnieją 3 rodzaje bloków:

  * blok kontrolny - bazowe informacje związane z plikiem
  * blok indeksowy - indeksy bloków danych i następnego bloku indeksowego
  * blok danych - po prostu zapisywane do pliku informacje

Rozmiar pojedynczego bloku to 512 bajtów.


Sposób użycia
-------------

Wystarczy użyć polecenie `make` i dołączyć bibliotekę do swojego programu,
przykładowo:

    make sefs
    cc -o sefs.o foo.c

Aby dowiedzieć się, w jaki sposób korzystać z biblioteki w swoim kodzie należy
udać się do pliku nagłówkowego (`sefs.h`) lub przejrzeć przykładowy program
testujący (`test.c`).


Protokół testowania
-------------------

Biblioteka daje się skompilować bez ostrzeżeń przy pomocy kompilatorów *clang*
oraz *gcc* z najwyższymi stopniami ostrzeżeń (`-Wall`, `-Wextra`, `-pedantic`).

Program testujący znajduje się w pliku `test.c`, poprawność wykonywania jest
sprawdzana za pomocą szeregu asercji. Program składa się z zestawu pięciu
testów, które badają różne aspekty biblioteki. Po uruchomieniu programu,
wszystkie asercje przechodzą i program bezbłędnie kończy swoją pracę.


### Inicjowanie i montowanie
Test tworzy przykładowy system plików o rozmiarze 1 MB w pliku "disk.dat" i
montuje go w celu wykorzystania do pozostałych testów. Asercje sprawdzają czy
zwrócony uchwyt do systemu plików ma poprawnie zainicjowane pola (np. czy
rozmiar systemu zgadza się z tym którego zażądano).

### Przydział plików i wyszukiwanie
Test otwiera trzy pliku, z czego utworzone jako nowe powinny zostać tylko dwa
pierwsze. Trzeci z nich powinien być tym samym plikiem który otwarto jako
pierwszy (asercja porównuje wskaźniki na bloki kontrolne plików - powinny być
takie same).

### Pisanie niewielkich danych tekstowych
Test zapisuje dane z niewielkiego wejściowego bufora tekstowego do pliku, a
następnie czyta dane z pliku do bufora wyjściowego. Asercja sprawdza czy
zawartość bufora wejściowego równa jest buforowi wyjściowemu.

### Pisanie dużych danych liczbowych
Test zapisuje do pliku prosty, ale długi ciąg liczbowy (tak duży aby plik
zajmował wiele bloków indeksowych). Następnie wczytuje ciąg z pliku i sprawdza
czy dane zgadzają się z ciągiem zapisywanym.

### Testowanie przydziału i zwalniania pamięci
Test tworzy i usuwa kilka plików, zapisując ilość wolnej pamięci po każdej z
operacji. Następnie asercje sprawdzają czy relacje są poprawne: przy tworzeniu
plików wolna przestrzeń powinna maleć, zaś przy usuwaniu - rosnąć.
