Trzecia pracownia z analizy numerycznej.
===

### Łukasz Hanuszczak (247952)

Kod zawiera bardzo mało komentarzy, ponieważ starałem się tworzyć go tak aby sam się komentował. Wszystkie nazwy próbowałem dobierać tak, aby osoba znająca angielski mogła zrozumieć za co dana część odpowiada. Całość jest podzielona na moduły co powinno ułatwić orientację. Jeżeli chodzi o implementację metod całkowania numerycznego, to wszystkie zostały zamknięte w oddzielnych klasach dziedziczących po wspólnym interfejsie ze względu na czytelność.

Program jest nieskomplikowany dzięki wykorzystaniu nowego standardu C++11. Zawiera on bardzo wiele oszczędzających czas i nerwy usprawnień, między innymi nadużywane przeze mnie funkcje anonimowe (wyrażenia lambda). Co prawda jest on przez różne kompilatory wspierany w różnym stopniu, jednak program powinien bez problemu dać radę się skompilować nawet przy pomocy wersji g++ z 2010 roku, a także na nowszych wersja kompilatora Microsoftu. U mnie na nienajnowszym g++ z 2011 program prawidłowo kompiluje się bez jakichkolwiek ostrzeżeń i błędów (nawet z flagą `-Wall` i `-pedantic`).

Sposób kompilacji (przy użyciu `g++ 4.6.3` dostępnej między innymi w standardowych repozytoriach Ubuntu):

  g++ -std=c++0x -Wall -pedantic -O2 -o program integral.cpp program.cpp


Składnia uruchamiania:

  ./program method:{M} function:{F} k:{K} [a:A b:B c:C d:D] [error] [series] [n:N]

gdzie:
  * `M` - to jedna z metod całkowania, do wyboru: romberg, gauss, trapezoids, simpson
  * `F` - to jedna z funkcji testowych, listę można zobaczyć w pliku nagłówkowym `functions.hpp`
  * `K` - ilość podprzedziałów (wierszy w przypadku tablicy Romberga)

parametry opcjonalne:
  * `A`, `B`, `C`, `D` - przedziały całkowania, domyślnie ustawione na kolejno 0, 1, 0, 1
  * `error` - zamiast wyniku drukowana jest wartość błędu w porównaniu do wyniku przyjętego jako dokładny
  * `series` - zamiast drukować wynik dla parametru k, drukuje listę wyników kolejno dla 1, 2..., k
  * `N` - ma zastosowanie tylko w przypadku kwadratury Gaussa, ustawia ilość węzłów

Przykładowe wywołania:

  ./program method:romberg function:testa1 k:10

  ./program method:gauss function:test3 k:100 error series n:5 a:-1 b:1 c:-1 d:1
