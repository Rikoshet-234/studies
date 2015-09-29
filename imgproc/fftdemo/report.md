% Pracownia z FFT
% Łukasz Hanuszczak
% 30 marca 2015


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

Jako pierwszy parametr wywołania należy podać obrazek wejściowy, natomiast drugim, opcjonalnym, parametrem jest obrazek wyjściowy (ścieżka do zapisu pliku).

~~~
$ fftdemo img/lena.jpg
$ fftdemo img/lena.jpg img/output.bmp
~~~

### Użytkowanie

Klawisze od *0*-*8* pozwalają wybrać przekształcenie transformaty (gdzie *0* oznacza transformatę wejściową a *1*-*8* takie jak w treści zadania).

Klawiszem *shift* (dowolnym) można dokonać przekształcenia $(-1)^{m + n}$ obrazka.

Klawisze *N*, *A*, *B*, *R*, *F* pozwalają ustawić tryb wyświetlania (gdzie *N* to po prostu obrazek, a pozostałe to widoki transformat takie jak w treści zadania).

Klawiszami *góra*/*dół* można manipulować skalą podczas wyświetlania transformat.

Klawiszami *prawo*/*lewo* można manipulować jasnością.

Klawisz *S* zapisuje aktualny widok do pliku podanego jako drugi parametr wywołania.


Przekształcenie $(-1)^{m + n}$ obrazka
--------------------------------------

Zgodnie z tym co było udowodnione na ćwiczeniach, transformata jest po prostu przesunięta (cyklicznie) o połowę wysokości i szerokości.


Przekształcenia transformat
---------------------------

W poniższych przykładach wykorzystywane są oznaczenia: $\mathcal{F}$ dla transformaty Fouriera, $\mathcal{F}^{-1}$ dla odwrotnej transformaty Fouriera, $\mathcal{I}$ dla obrazka wejściowego, $\mathcal{I}^\mathbf{R}$ dla obrazka wejściowego obróconego o $180^{\circ}$, $\Re$ i $\Im$ dla funkcji mapujących obrazki na odpowiednio ich części rzeczywiste i urojone.

Ponadto, przydatny będzie również lemat mówiący że $\mathcal{F}^{-1} \left( \mathcal{F} \left( \Re \left( \mathcal{I} \right) - \Im \left( \mathcal{I} \right) \right) \right) = \mathcal{I}^{\mathbf{R}}$. Bez wdawania się w zagmatwane obliczenia, nieformalnie można powiedzieć że sprzężenie liczb zespolonych odwraca ich fazy (tzn. jeżeli $x$ miało fazę $\phi$ to $\bar{x}$ ma fazę $-\phi$). Więc skoro fale mają teraz ,,obrócone'' fazy to sumaryczny obrazek składający się z tych fal również będzie obrócony.

### Zerowanie części rzeczywistej

Wynik składa się z dwóch zmieszanych warstw: jednej z oryginalnym obrazkiem oraz drugiej z obrazkiem ,,do góry nogami''. Zjawisko to łatwo wyjaśnić prostymi przekształceniami korzystającymi z powyższego lematu.

$$
\mathcal{F}^{-1} \left(
    \Re \left( \mathcal{F} \left( \mathcal{I} \right) \right)
\right)
=
\frac{1}{2} \mathcal{F}^{-1} \left(
    2 \Re \left( \mathcal {F} \left( \mathcal{I} \right) \right)
\right)
=
$$
$$
=
\frac{1}{2} \mathcal{F}^{-1} \left[
    \Re \left( \mathcal {F} \left( \mathcal{I} \right) \right) +
    \Im \left( \mathcal {F} \left( \mathcal{I} \right) \right) +
    \Re \left( \mathcal {F} \left( \mathcal{I} \right) \right) -
    \Im \left( \mathcal {F} \left( \mathcal{I} \right) \right)
\right]
=
$$
$$
=
\frac{1}{2} \mathcal{F}^{-1} \left[
    \mathcal{F} \left( \Re \left( \mathcal{I} \right) +
                       \Im \left( \mathcal{I} \right) \right) +
    \mathcal{F} \left( \Re \left( \mathcal{I} \right) -
                       \Im \left( \mathcal{I} \right) \right)
\right]
=
$$
$$
=
\frac{1}{2} \left[
    \mathcal{F}^{-1} \left( \mathcal{F} \left(
        \Re \left( \mathcal{I} \right) +
        \Im \left( \mathcal{I} \right)
    \right) \right) +
    \mathcal{F}^{-1} \left( \mathcal{F} \left(
        \Re \left( \mathcal{I} \right) -
        \Im \left( \mathcal{I} \right)
    \right) \right)
\right]
=
$$
$$
=
\frac{1}{2} \left[
    \mathcal{F}^{-1} \left( \mathcal{F} \left(
        \mathcal{I}
    \right) \right) +
    \mathcal{F}^{-1} \left( \mathcal{F} \left(
        \Re \left( \mathcal{I} \right) -
        \Im \left( \mathcal{I} \right)
    \right) \right)
\right]
=
\frac{1}{2} \left( \mathcal{I} + \mathcal{I}^{\mathbf{R}} \right)
$$

### Zerowanie części urojonej

Podobnie jak poprzednio: dwie nakładające się na siebie warstwy. Różnica polega na tym, że w tym wypadku odwrócony obrazek wydaje się być dodatkowo negatywem. Wyjaśnienie jest niemal identyczne jak powyżej:

$$
\mathcal{F}^{-1} \left(
    \Im \left( \mathcal{F} \left( \mathcal{I} \right) \right)
\right)
=
\frac{1}{2} \mathcal{F}^{-1} \left(
    2 \Im \left( \mathcal {F} \left( \mathcal{I} \right) \right)
\right)
=
$$
$$
=
\frac{1}{2} \mathcal{F}^{-1} \left[
    \Re \left( \mathcal {F} \left( \mathcal{I} \right) \right) +
    \Im \left( \mathcal {F} \left( \mathcal{I} \right) \right) -
    \Re \left( \mathcal {F} \left( \mathcal{I} \right) \right) +
    \Im \left( \mathcal {F} \left( \mathcal{I} \right) \right)
\right]
=
$$
$$
=
\frac{1}{2} \mathcal{F}^{-1} \left[
    \mathcal{F} \left( \Re \left( \mathcal{I} \right) +
                       \Im \left( \mathcal{I} \right) \right) -
    \mathcal{F} \left( \Re \left( \mathcal{I} \right) -
                       \Im \left( \mathcal{I} \right) \right)
\right]
=
$$
$$
=
\frac{1}{2} \left[
    \mathcal{F}^{-1} \left( \mathcal{F} \left(
        \Re \left( \mathcal{I} \right) +
        \Im \left( \mathcal{I} \right)
    \right) \right) -
    \mathcal{F}^{-1} \left( \mathcal{F} \left(
        \Re \left( \mathcal{I} \right) -
        \Im \left( \mathcal{I} \right)
    \right) \right)
\right]
=
$$
$$
=
\frac{1}{2} \left[
    \mathcal{F}^{-1} \left( \mathcal{F} \left(
        \mathcal{I}
    \right) \right) -
    \mathcal{F}^{-1} \left( \mathcal{F} \left(
        \Re \left( \mathcal{I} \right) -
        \Im \left( \mathcal{I} \right)
    \right) \right)
\right]
=
\frac{1}{2} \left( \mathcal{I} - \mathcal{I}^{\mathbf{R}} \right)
$$

### Zastępowanie normą

Na obrazku pozostaje jedynie bliżej niesprecyzowany szum (dla fotografii, dla prostych geometrycznych kształtów można zaobserwować intrygujące mozaiki). Zatem wzięcie normy gubi dość istotne informacje i wydaje się że nie powinno prowadzić do żadnego konkretnego rezultatu.

### Normowanie

Po ustawieniu bardzo dużego wzmocnienia wynikowego obrazu można zobaczyć mocno zaszumione zbiory punktów głównie w obszarach o dużej częstotliwości, więc daje to efekt prostego wykrywania krawędzi. W połączeniu z wiedzą z poprzedniego przykładu pozwala to wyciągnąć wniosek że to właśnie faza niesie ze sobą bardziej istotne dla obrazka informacje.

### Zastępowanie sprzężeniem

Wynikowy obrazek jest obrócony o $180^{\circ}$. Wynika to bezpośrednio z wprowadzonego na początku lematu.

### Zamiana $a + b \imath$ na $0.25a + b \imath$

Znowu są dwa nakładające się warstwy podobnie jak przy zerowaniu części rzeczywistej i przy zerowaniu części urojonej. Uzasadnienie jest identyczne, tylko tutaj część rzeczywista zostaje z małą ,,wagą'' zamiast zostać zupełnie wyzerowaną.

### Mnożenie przez $m^2 + n^2$

Na obrazku zdecydowanie zwiększa się kontrast. Gdyby mnożenie odbywało się przez jakąś stałą, łatwo można wtedy uzasadnić to liniowością transformaty. Przy zmiennych wartościach $m$ i $n$ jest podobnie tylko ,,słabiej'' dla małych $m$ i $n$ a ,,mocno'' dla dużych.

### Zerowanie w kwadracie

Po zastosowaniu filtra dostajemy obrazek który jest bardzo rozmyty. Powodem jest oczywiście usunięcie wysokich częstotliwości: niskie częstotliwości znajdują się w kwadracie, natomiast wszystkie inne zostają zniwelowane.


Przykładowe wyniki
------------------

W dalszej części tego sprawozdania znajdują się graficzne wyniki przeprowadzonych eksperymentów. Różne widoki transformat zaprezentowane są tylko dla obrazka wejściowego -- dla wyników przekształceń pokazane są jedynie wyjściowe obrazki, ponieważ same transformaty nie obrazują żadnych ciekwych efektów.

![Lena, obrazek wejściowy](../img/lena_0.png)

![Lena, obrazek wejściowy, część rzeczywista transformaty](../img/lena_normal_a.png)

![Lena, obrazek wejściowy, część urojona transformaty](../img/lena_normal_b.png)

![Lena, obrazek wejściowy, moduł transformaty](../img/lena_normal_r.png)

![Lena, obrazek wejściowy, faza transformaty](../img/lena_normal_f.png)

![Lena, obrazek $(-1)^{m + n}$, moduł transformaty](../img/lena_shift_r.png)

![Lena, przekształcenie zerujące część rzeczywistą](../img/lena_1.png)

![Lena, przekształcenie zerujące część urojoną](../img/lena_2.png)

![Lena, przekształcenie zastępujące normą](../img/lena_3.png)

![Lena, przekształcenie normujące, wzmocnione](../img/lena_4.png)

![Lena, przekształcenie zastępujące sprzężeniem](../img/lena_5.png)

![Lena, przekształcenie zamieniające $a + b \imath$ na $0.25a + b \imath$](../img/lena_6.png)

![Lena, przekształcenie wymnażajace przez $m^2 + n^2$, przeskalowane](../img/lena_7.png)

![Lena, przekształcenie zerujące w kwadracie](../img/lena_8.png)

![Kwadrat, obrazek wejściowy](../img/square.jpg)

![Kwadrat, przekształcenie zastępujące normą](../img/square_3.png)

![Kwadrat, przekształcenie normujące, wzmocnione](../img/square_4.png)
