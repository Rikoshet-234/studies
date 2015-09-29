% Pracownia z filtrów
% Łukasz Hanuszczak
% 20 kwietnia 2015


Program
-------

### Kompilacja

Do kompilacji wykorzystywany jest system budowania *CMake*. Będąc w głównym folderze z programem należy wykonać następujące polecenia:

~~~
$ mkdir build
$ cd build
$ cmake ..
$ make
~~~

### Uruchamianie

Najbardziej ogólnie, składnia wywołania programu wygląda następująco:

~~~
$ filters <task> <size> [output] [input]
~~~

Gdzie `task` specyfikuje podpunkt z treści zadania (`a1`, `a2`, `a3`, `a4`, `a5`, `b1`, `b2`, `b3`), `size` to rozmiar filtra. Opcjonalny parametr `output` pozwala określić gdzie zapisać wygenerowany obrazek (w przypadku jego braku zostają wykonane obliczenia, ale ich wynik jest ignorowany), zaś opcjonalny parametr `input` specyfikuje ścieżkę do obrazka wejściowego (w przypadku jego braku używany jest obrazek generowany tak jak opisane w treści zadania).

Dodatkowo, udostępnione są jeszcze skrypty `benchmark` i `test`. Pierwszy z nich uruchamia serię testów wydajnościowych dla podpunków z części *A* pracowni z różnymi rozmiarami filtra. Drugi generuje obrazki dla części *B* pracowni: jako parametry przyjmuje folder wyjściowy oraz (opcjonalnie) obrazek wejściowy.

Przykładowe wywołania:

~~~
$ filters a2 30
$ filters b2 10 out.bmp
$ filters b1 20 out.bmp img/lena.jpg
$ benchmark
$ test img/ img/lena.jpg
~~~


Część *A*
---------

Poniższe wyniki zostały uzyskane na netbooku z APU C-50 (1 GHz), na ,,normalnych'' komputerach program działa dużo szybciej. Dość istotnym czynnikiem jest kompilacja z flagą `-O2`. Nawet dla filtrów rozmiaru $20$ w przypadku splotu z definicji daje to ponad dziesięciokrotne przyspieszenie, więc biorąc pod uwagę złożoność algorytmu, dla wyższych rozmiarów filtra ta różnica może być jeszcze większa.

 $n$    `a1`    `a2`    `a3`    `a4`    `a5`
---- ------- ------- ------- ------- -------
  5   0.359s  0.321s  0.551s  0.575s  0.779s
 10   0.820s  0.405s  0.709s  0.988s  1.210s
 20   2.643s  0.638s  1.397s  0.853s  1.046s
 25   4.068s  0.743s  1.931s  0.739s  0.953s
 30   5.475s  0.848s  2.075s  0.986s  1.421s
 50  14.102s  1.212s  3.439s  0.782s  1.006s
100  53.118s  2.176s  6.882s  0.880s  1.037s

Table: Porównanie wydajności różnych metod obliczania splotu.

Wyniki nie są zbyt zaskakujące. Wnioski jakie można wyciągnąć są takie, że dla małych (rozmiaru ~30 w przypadku netbooka) filtrów separowalnych najbardziej opłaca się wykonać dwa sploty 1D. Dla dużych wartości, opłaca się używać FFT 2D dla którego wielkość filtrów za bardzo nie ma znaczenia i w każdym przypadku zajmuje to mniej-więcej tyle samo czasu. Jeżeli używane jest FFT to nawet dla filtrów separowalnych zupełnie nie ma sensu wykonywać dwóch splotów 1D.


Część *B*
---------

Poniżej zaprezentowane są wyniki działania filtru dwumianowego, prostokątnego oraz wykrywanie krawędzi przy pomocy operatora Laplace'a (w wersji z uwzględnieniem ,,skośnych'' sąsiadów) na przykładowych obrazkach.

Na teksturze z koncentrycznymi falami można zaobserwować wyraźną różnicę pomiędzy filtrem dwumianowym a filtrem prostokątnym: ten drugi istotnie ,,gubi'' pewne informacje. Natomiast po zastosowaniu operatora Laplace'a w rogach (miejscach o wysokiej częstotliwości ,,na ukos'') pojawiają się artefakty przypominające dodatkowe okręgi. Jedynie po zostosowaniu filtra dwumianowego obrazek zachowuje swoje oryginalne właściwości - skąd wniosek o jego anizotropowości.

\begin{figure}[htbp]
\centering
\includegraphics[scale=0.7]{../img/out_waves_b1.png}
\caption{Fale, filtr dwumianowy}
\end{figure}

\begin{figure}[htbp]
  \centering
  \includegraphics[scale=0.7]{../img/out_waves_b2.png}
  \caption{Fale, filtr prostokątny}
\end{figure}

\begin{figure}[htbp]
  \centering
  \includegraphics[scale=0.7]{../img/out_waves_b3.png}
  \caption{Fale, operator Laplace'a}
\end{figure}

\begin{figure}[htbp]
  \centering
  \includegraphics[scale=0.7]{../img/out_custom_b1.png}
  \caption{Lena, filtr dwumianowy}
\end{figure}

\begin{figure}[htbp]
  \centering
  \includegraphics[scale=0.7]{../img/out_custom_b2.png}
  \caption{Lena, filtr prostokątny}
\end{figure}

\begin{figure}[htbp]
  \centering
  \includegraphics[scale=0.7]{../img/out_custom_b3.png}
  \caption{Lena, operator Laplace'a}
\end{figure}
