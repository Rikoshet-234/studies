Druga pracownia z analizy numerycznej
===

### Łukasz Hanuszczak (247952)

Program nie jest optymalizowany pod zużycie pamięci. Zdecydowałem
jednak postawić na czytelność i wierne oddanie w kodzie formuł
wyprowadzonych ze sprawozdania. Mam więc świadomość tego, że w wielu
miejscach użycie tablic jest kompletnie zbędne i można je zastąpić
dwoma zmiennymi.

Najlepiej kompilować przy pomocy:
  g++ -Wall -pedantic -std=c++0x -o program program.cpp

Składnia wykonania:
  ./program n:{N} method:{M} function:{F} [deriv] polynomial:{P} [error]

gdzie:
  * `N` to ilość węzłów interpolacji (domyślnie `10`)
  * `M` to metoda ewaluacji kombinacji liniowej, możliwe parametry to
    `clenshaw`, `forsythe`
  * `F` to funkcja która będzie ewaluowana, możliwe parametry to
    `runge`, `simple`, `sin`, `exp`
  * `P` to wielomian interpolujący, możliwe parametry to
    `I`, `J`

parametry opcjonalne:
  * `deriv` - ustawia flagę, że liczona będzie pochodna danej funkcji
  * `error` - zamiast wartości interpolacji drukuje błąd z wartości dokładnej

Kolejność argumentów nie ma znaczenia. Przykładowe wywołania:

  ./program n:100 method:forsythe function:runge deriv polynomial:I

  ./program method:clenshaw function:exp polynomial:J error
