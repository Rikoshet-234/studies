HShell, generator skryptów
==========================


Informacje ogólne
-----------------

Powłoka systemowa jest świetnym narzędziem do wydawania systemowi krótkich,
zwięzłych poleceń. Gdy jednak zaczniemy pisać dłuższe skrypty, zaczynają się
problemy - głównie z powodu dynamicznej natury języka. Błędy dają o sobie znać
dopiero w momencie uruchomienia skryptu (a czasem nawet pozostają ukryte przez
dłuższy czas z powodu różnych ścieżek wykonania). Większość tego typu błędów,
to nie błędy logiczne programu lecz zwykłe literówki, niepoprawne parametry
wywołania i inne drobnostki, które z łatwością mogłyby zostać wychwycone gdyby
język oferował statyczne typowanie.

Tutaj z pomocą przychodzi *HShell*, generator bezpiecznych skrytów napisany w
Haskellu - najbardziej, statycznym i bezpiecznym języku jaki istnieje.
Ciężko krótko i odpowiednio stwierdzić czym właściwie *HShell* jest. Można go
rozumieć jako swego rodzaju ramę, oferującą bardzo zbliżony pewien *DSL*,
z którego następnie można wygenerować gotowy kod bashowego skryptu.


Sposób użycia
-------------

### Podstawy
Każdy skrypt jest typu `ShellScript a`. `ShellScript` jest monadą, co umożliwia
wygodne, zbliżone do rdzennych, łączenie skryptów. HShell oferuje gotowy zbiór
poleceń powłoki Bash (który można w prosty sposób rozszerzać). Przykładowo,
dostępne są między innymi `ls`, `wc`, `grep`, `echo`. Każde z poleceń posiada
wersję z domyślnymi parametrami wywołania - wystarczy dodać do polecenia znak
apostrofu (`ls'`, `wc'`). W przypadku braku tego apostrofu, należy nadpisać
zbiór domyślnych argumentów przy pomocy funkcji `with`. Argumenty te, mniej
lub bardziej odpowiadają analogicznym parametrom w oryginalnych poleceniach
dostępnych w powłoce systemowej.


    script :: ShellScript ()
    script = do
        echo' ["Witaj swiecie"]
        echo (with { echoNewline = False }) ["Bez nowej linii"]
        ls' [".", "folder1/", "folder2/"]


### Składanie poleceń
Bardzo przydatnym narzędziem powłoki jest *składanie* poleceń (ang.
*pipelining*). Do budowania potoku poleceń używamy operatora
`(~>) :: ShellScript () -> ShellPipeline -> ShellPipeline`. Aby przekształcić
potok w skrypt można użyć funkcji `pipe :: ShellPipeline -> ShellScript ()`.
Przykładowo, do zliczenia ilości plików z kropką w bieżącym folderze można
wykorzystać następujący skrypt:


    script :: ShellScript ()
    script = do
        pipe $ ls' ["."]
            ~> grep' "\\." []
            ~> wc (with { wcMode = CountLines }) []
            ~> []


Ostatecznie, kod źródłowy tak przygotowanego skryptu można otrzymać używając
funkcji `source :: ShellScript a -> String`.


    main :: IO ()
    main = do
        let script = echo' ["Prosty skrypt"]
        putStrLn $ source script


Rozszerzanie zbioru poleceń
---------------------------

Aby utworzyć nowe polecenie, wystarczy utworzyć funkcję zwracającą obiekt typu
`BashScript a`. Jeżeli funkcja ma przyjmować opcjonalne argumenty, typ tych
argumentów powinien być zainstalowany w klasie `ShellOpts`, czyli udostępniać
obiekt `defaultOpts`, będący domyślnymi parametrami wywołania.
