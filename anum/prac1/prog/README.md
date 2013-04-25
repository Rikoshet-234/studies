Pierwsza pracownia z analizy numerycznej
===

### Łukasz Hanuszczak (247952)

Najlepiej kompilować przy pomocy komendy:
  g++ -Wall -pedantic -std=c++0x -o program program.cpp

Składnia wykonania:
  ./program frac:{FRAC} method:{METHOD} n:{N} [bare] [series] [error]

gdzie:
  * FRAC to wybór ułamka do obliczenia, możliwe parametry to
    pi, four_pi, pi_four, e, atan odpowiadające definicjom w kodzie
  * METHOD to metoda obliczania ułamka, możliwe parametry to
    upstairs (up, u), downstairs (down, d)
  * N to liczba iteracji czy też wysokość ułamka jak kto woli

parametry opcjonalne:
  * bare - drukuje wartości bez upiększaczy, do generowania wykresów
  * series - nie oblicza wartości tylko dla n, lecz dla 1..n
  * error - zamiast wartości ułamka drukuje błąd z wartości dokładnej

Kolejność argumentów nie ma znaczenia. Przykładowe wywołania:

./program frac:pi n:3 method:up series
n    | value
0001 | 3.1666667461395264
0002 | 3.1333334445953369
0003 | 3.1452381610870361

./program frac:e n:6 method:d
n    | value
0006 | 2.7182633876800537

./program frac:arctan n:10 method:upstairs bare error
(...)
