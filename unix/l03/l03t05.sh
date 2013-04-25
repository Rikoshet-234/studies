
# Katalog na pliki tymczasowe, który zostanie usunięty przy wyjściu.
TMPD=`mktemp -d`
trap "rm -rf $TMPD" EXIT
# Format po złączeniu:
# indeks, imię, nazwisko, wykład, komentarz, pracownia, komentarz
FORMAT='1.3 1.1 1.2 1.4 1.5 2.4 2.5 2.3 2.1 2.2'

# Parsowanie opcjonalnych parametrów.
while [ $# -gt 0 ] && expr "$1" : "-.*$" > /dev/null
do
	opt=$1
	shift
	case "$opt" in
	"-w")
		SRED_WZWYZ=1
		;;	
	"-p")
		WAGA_PRAC=$1
		shift
		;;
	"-k")
		WAGA_KOLO=$1
		shift
		;;
	"-fk")
		FK=$1
		shift
		;;
	"-fp")
		FP=$1
		shift
		;;
	*)
		echo "Invalid option: $opt" > /dev/tty
		exit 1
		;;
	esac
done


# Sprawdź czy nie brakuje któregoś z potrzebnych parametrów.

if [ -z "$WAGA_KOLO" ]
then
	echo -n "Podaj wagę kolokwium: " > /dev/tty
	read WAGA_KOLO < /dev/tty > /dev/tty
fi

if [ -z "$WAGA_PRAC" ]
then
	echo -n "Podaj wagę pracowni: " > /dev/tty
	read WAGA_PRAC < /dev/tty > /dev/tty
fi

if [ -z "$FK" ] && [ -z "$FP" ]
then
	# Nie został podany żaden parametr opcjonalny, więc wczytuje z normalnych argumentów.
	if [ -z "$1" ] && [ -z "$2" ]
	then
		echo "Brak przynajmniej jednego pliku wejściowego." > /dev/tty
		exit 1
	fi
	FK=$1
	FP=$2
elif [ -z "$FK" ]
then
	# Nie został podany plik z kolokwiami, wczytujemy z wejścia.
	cat > "$TMPD/k.pre"
	FK="$TMPD/k.pre"
elif [ -z "$FP" ]
then
	# Nie został podany plik z pracowniami, wczytujemy z wejścia.
	cat > "$TMPD/p.pre"
	FP="$TMPD/p.pre" 
fi


# Sortowanie danych wejściowych, aby `join` działał prawidłowo.
sort -t ',' -k 3 "$FK" > "$TMPD/k.sort"
sort -t ',' -k 3 "$FP" > "$TMPD/p.sort"

# Łączenie danych i ładne wypisywanie.
join -t ',' -a 1 -a 2 -1 3 -2 3 -o "$FORMAT" "$TMPD/k.sort" "$TMPD/p.sort" |
nawk -F ',' -v k="$WAGA_KOLO" -v p="$WAGA_PRAC" -v w="$SRED_WZWYZ" '
function round(x) {
	return int(x + 0.5)
}

function ceil(x) {
	return (x == int(x)) ? x : int(x) + 1
}

{
	id = $1 != "" ? $1 : $8
	fname = $2 != "" ? $2 : $9
	sname = $3 != "" ? $3 : $10

	printf "+ %s %s (%d)\n", fname, sname, id
	
	# Oceny cząstkowe.
	g = match($4, "^[0-9]+") ? sprintf("%.1f", $4) : "-  "
	printf "  - wykład:    " g " (%s)\n", $5

	g = match($6, "^[0-9]+") ? sprintf("%.1f", $6) : "-  "
	printf "  - pracownia: " g " (%s)\n", $7

	# Ocena końcowa.
	r = ($4 < 3.0 || $6 < 3.0) ? 2.0 : ((k * $4 + p * $6) / (k + p))
	# Zaokrąglij zgodnie z wymogami.
	r = w ? (ceil(r * 2) / 2.0) : (round(r * 2) / 2.0)

	printf "  - końcowa:   " "%.1f\n", r
}
'
