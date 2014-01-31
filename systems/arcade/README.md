Salon gier
==========


Informacje ogólne
-----------------

*Salon gier* to program obrazujący synchronizację między procesami przy użyciu
uniksowych semaforów napisany na pracownię z *Systemów operacyjnych*. Ilość
komentarzy jest ograniczona do niezbędnego minimum ze względu na prostotę kodu.

Podróż przez źródła programu polecam zacząć od zapoznania się z plikami
nagłówkowymi a dopiero później zagłębić się w szczegóły implementacji, która
jest napisana w sposób stosunkowo jasny i oczywisty dla każdej osoby znającej C
i podstawy uniksowych bibliotek. A przynajmniej taką mam nadzieję.


Fabuła
------

Całość dzieje się gdzieś w Polsce w latach 90., kiedy salony gier cieszyły
się całkiem sporym zainteresowaniem.

Każdy dzień rozpoczyna się od wejścia właściciela (w tej roli proces główny),
który to kolejno uruchamia automaty do gier, wybiera czystą stronę w rejestrze
gości i sprząta ladę. Po upewnieniu się że wszystko jest jak należy, otwiera
salon.

Następnie, do salonu zaczynają powoli wlewać się kolejni gracze (w tej roli
procesy potomne). Każdy z nich ma kilka monet w portfelu i plan na dany dzień,
czyli gry jakie musi wygrać aby być szczęśliwym człowiekiem.

Ponieważ automaty wymagają żetonów, wszyscy gracze musza się najpierw w nie
zaopatrzyć. W tym celu próbują dopchać się (pojęcie kultury jest im raczej
obce) do lady przy której obsługiwany może być oczywiście maksymalnie tylko
jeden klient. Gdy w końcu uda się dostać do kasy, gracz wykłada na tacę
tyle monet ile żetonów chce kupić, a kupić chce tyle ile gier ma w planie do
wygrania (bo i po co więcej?). Następnie właściciel bierze pieniądze,
zostawiając na tacy żetony które podnosi podekscytowany gracz.

Tak wyposażony klient może zacząć zabawę. Rusza w stronę najbliższego automatu
znajdującego się na jego harmonogramie i, analogicznie do lady, próbuje się do
niego dopchać (tutaj również trzeba mieć odrobinę szczęścia). Gdy już uda mu
się stanąć przed wymarzoną maszyną, wrzuca żeton i rozpoczyna grę. Może się
ona oczywiście skończyć wygraną lub przegraną. W przypadku pierwszym odchacza
grę ze swojej listy i kontynuuje zgodnie z planem. W drugim musi spróbować
ponownie. Zarówno po wygranej jak i po przegranej może mu zabraknąć żetonów.
Wtedy (o ile pozwala mu na to zawartość portfela) znowu udaje się w kierunku
lady. Gracz opuszcza salon gdy wygra wszystkie zaplanowane przez siebie gry
lub gdy skończą mu się środki na kupno nowych żetonów.

Każdy z klientów opuszczając salon robi odpowiedni wpis w rejestrze. Ostatni
z graczy jest zobowiązany poinformować o swoim wyjściu właściciela. Wtedy ten
może zamknąć salon, wyłączyć maszyny i opuścić salon.


Sposób użycia
-------------

Program skompilować można przy użyciu polecenia

    make

Po skompilowaniu wystarczy uruchomić program

    ./arcade 4

gdzie 4 to przykładowa ilość klientów odwiedzających sklep (program uruchomiony
bez parametru ustawi tę wartość domyślnie na 2).


Protokół testowania
-------------------

Program testowany jest w dwojaki sposób: przy pomocy asercji w kodzie źródłowym
oraz poprzez obserwację wyników urachamiania.

### Asercje
Asercje upewniają się że wszystkie reguły *salonu* pozostają nienaruszone
(gracz ustawia się w kolejce tylko gdy ma wymaganą ilość żetonów, nie można
ustawiać się w kolejce do niedziałającego automatu, gracz gra tylko w gry które
ma na swojej liście, wymianie ulega tylko dodadnia ilość żetonów itd.). W ten
sposób upewniamy się że gracze nie wykonują irracjonalnych decyzji i chociaż
częściowo postępują w sposób zgodny z wymaganiami zadanfia.

### Obserwacja wyników
Trudniej jest jeżeli chodzi o faktyczne sprawdzanie czy procesy się poprawnie
synchronizują - niektóre z tego typu błędów mogą być wyłapane przez asercje (na
przykład jeżeli dwóch graczy na raz zechce wymienić żetony to może wystąpić
sytuacja że na ladzie będzie ich ujemna ilość), lecz nie wszystkie. W tym celu
program wypisuje na wyjściu aktualnie podejmowaną przez gracza (lub właściciela
salonu) czynność. Na podstawie analizy wyjścia można stwierdzić z dużym
prawdopodobieństwem, że program działa poprawnie. Oczywiście, ponieważ procesy
przeplatają się w sposób niedeterministyczny, nie można tego zawsze zapewnić.
