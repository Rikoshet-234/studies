#include <cstdio>
#include <algorithm>

typedef unsigned int uint;
typedef unsigned long long uint64;

#define INF (1LL * 'I' * 'N' * 'F' * 'T' * 'Y')
#define MAX_N 200

uint p[MAX_N + 1]; /* Rozmiary macierzy. */
uint64 opt[MAX_N][MAX_N]; /* Najniższe koszty mnożenia macierzy */

void eval(const uint &i, const uint &j)
{
	opt[i][j] = +INF;
	for (uint l = i; l < j; l++) {
		opt[i][j] = std::min(
			opt[i][j],
			opt[i][l] + opt[l + 1][j] + p[i] * p[l + 1] * p[j + 1]
		);
	}
}

void fill(const uint &n)
{
	for (uint i = 0; i < n; i++) {
		opt[i][i] = 0;
	}

	for (uint a = 1; a < n; a++) {
		for (uint b = 0; b < n - a; b++) {
			eval(b, b + a);
		}
	}
}

void test_case()
{
	/* Wczytywańsko danych. */
	uint n;
	scanf("%u", &n);

	for (uint i = 0; i <= n; i++) {
		scanf("%u", &p[i]);
	}

	/* Wypełniańsko tablicy optymalnych rozwiązań. */
	fill(n);

	/* I wypisywanńsko odpowiedzi. */
	printf("%llu\n", opt[0][n - 1]);
}

int main()
{
	uint T;
	scanf("%u", &T);

	for (uint t = 0; t < T; t++) {
		test_case();
	}

	return 0;
}
