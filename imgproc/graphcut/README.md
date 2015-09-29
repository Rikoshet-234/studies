GraphCut
========


Wymagania
---------

Do kompilacji potrzebny jest system CMake (testowane pod wersją 2.8), kompilator wspierający standard C++11 (testowane pod clang 3.4) oraz zainstalowana biblioteka SDL2.


Kompilacja
----------

    mkdir build
    cd build
    cmake ..
    make


Uruchamianie
------------

Składnia wywoływania:

    graphcut [OPTION...] FILE

gdzie możliwe opcje to:

    -a --algorithm ALG    gdzie ALG to jedno z: edmonds, boykov, boykov2
                          (domyślnie edmonds)
    -l --lambda LAM       gdzie LAM to pewna liczba rzeczywista
                          (domyślnie 0)


Obsługa
-------

Do rysowania maski obiektu używa się lewego przycisku myszy a do rysowania maski tła prawego. W celu wymazania zaznaczenia można użyć środkowego przycisku.

Klawiszem spacji uruchamia się procedurę wyliczania przepływu co może na pewien czas unieruchomić aplikację (w konsoli powinny zacząć wyświetlać się komunikaty diagnostyczne).

Po wyliczeniu maski klawiszem *S* można ukryć/pokazać zaznaczone obszary, zaś klawiszem *D* ukryć/pokazać wyliczoną segmentację.


Komentarz
---------

Uruchamianie programu z parametrem lambda innym niż 0 znacznąco pogarsza wydajność - dlatego w celu szybkiego testowania wskazane jest albo używanie właśnie 0 lub obrazków o małych rozmiarach (100x100 powinny liczyć się do 10 sekund z niezerową lambdą). W folderze `demos` znajduje się kilka przykładowych wyników (pliki `gc-*.png`) segmentacji dla lambd 0.5 - 1.0.

Algorytmy Boykova na ogół *nie* działają. Wersja pierwsza czasami się zapętla, czasami działa dobrze (`demos/boykov-noloop-*.png`). Wersja druga to wersja bez jakichkolwiek opymalizacji typu pamiętanie przepływu "do wierzchołka", używanie `std::set` zamiast kolejki i tablicy flag itp. oraz z wyłączoną adopcją - niestety z powodu tego daje złe rezultaty i np. wagi tła nie zawsze są uwzględniane. Ale też czasami daje całkiem dobre rezulaty (`demos/boykov2-lena-*.png`) a wyniki są natychmiastowe.
