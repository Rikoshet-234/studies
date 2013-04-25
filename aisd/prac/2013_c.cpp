/*
 * Łukasz Hanuszczak
 * numer indeksu: 247952
 * grupa: KLO
 */

#include <cstdio>

typedef long long int int64;
typedef unsigned int uint;

struct coin {
	int p; /* Nominał w groszach. */
	uint w; /* Waga w groszach. */
};

#define INF (1LL * 'I' * 'F' * 'I' * 'N' * 'I' * 'T' * 'Y')
#define MAX_F 100000
#define MAX_C 100

coin coins[MAX_C + 1]; /* Dostępne "w obiegu" monety. */
uint count[MAX_C + 1]; /* Ilości monet potrzebnych do optymalnej wartości. */ 
int64 Pmin[MAX_F + 1], Pmax[MAX_F + 1]; /* Najoptymalniejsza wartość pieniężna dla danego worka. */
uint Umin[MAX_F + 1], Umax[MAX_F + 1]; /* Indeks monety użytej do zrobienia optymalnej wartości. */

/**
 * Wylicza najmniejszą/największą możliwą do uzyskania wartość pieniężną
 * w woreczku o wadze `f` używając `c` pierwszych nominałów.
 * 
 * @param f Aktualnie rozpatrywana waga worka.
 * @param c Ilość aktualnie rozpatrywanych nominałów.
 */
inline void eval(const uint &f, const uint &c)
{
	if (f >= coins[c].w) {
		const int64 pmin = coins[c].p + Pmin[f - coins[c].w];
		const int64 pmax = coins[c].p + Pmax[f - coins[c].w];

		/* Sprawdzamy czy bardziej opłaca się użyć monety czy nie. */
		if (pmin < Pmin[f]) {
			Pmin[f] = pmin;
			Umin[f] = c;
		}

		if (pmax > Pmax[f]) {
			Pmax[f] = pmax;
			Umax[f] = c;
		}
	};
}

/**
 * Korzystając z programowania dynamicznego uzupełnia tablice `P`.
 *
 * @param F Maksymalny ciężar worka.
 * @param C Ilość monet "w obiegu".
 */
inline void fill(const uint &F, const uint &C)
{
	for (uint f = 1; f <= F; f++) {
		Pmin[f] = +INF;
		Pmax[f] = -INF;
	}

	for (uint c = 1; c <= C; c++) {
		for (uint f = 1; f <= F; f++) {
			eval(f, c);
		}
	}
}

/**
 * Korzystając z tablic `P` i `U` "ujawnia" zawartość woreczka.
 *
 * @param U Używana tablica `U` - rozkładamy dla minimum albo dla maksimum.
 * @param F Maksymalny ciężar worka.
 * @param C Ilość monet "w obiegu".
 */
inline void factor(const uint *U, const uint &F, const uint &C)
{
	uint f = F;

	for (uint c = 1; c <= C; c++) {
		count[c] = 0;
	}

	while (f != 0) {
		count[U[f]]++;
		f -= coins[U[f]].w;
	}
}

int main()
{
	/* Wczytywanie danych. */
	uint F;
	scanf("%u", &F);

	uint C;
	scanf("%u", &C);

	for (uint c = 1; c <= C; c++) {
		scanf("%d %d", &coins[c].p, &coins[c].w);
	}

	/* Wyliczanie optymalnych wartości. */
	fill(F, C);

	if (Pmin[F] == +INF) {
		printf("NIE\n");
		return 0;
	}
	printf("TAK\n");

	/* Odpowiedź dla minimum. */
	printf("%lld\n", Pmin[F]);

	factor(Umin, F, C);
	for (uint c = 1; c <= C; c++) {
		printf("%u ", count[c]);
	}
	printf("\n");

	/* Odpowiedź dla maksimum. */
	printf("%lld\n", Pmax[F]);

	factor(Umax, F, C);
	for (uint c = 1; c <= C; c++) {
		printf("%u ", count[c]);
	}
	printf("\n");

	return 0;
}
