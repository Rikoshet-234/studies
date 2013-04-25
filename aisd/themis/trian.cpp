#include <cstdio>
#include <algorithm>

typedef unsigned int uint;
typedef unsigned long long uint64;

#define INF (1LL * 'I' * 'N' * 'F' * 'I' * 'N' * 'I' * 'T' * 'Y')
#define MAX_N 200

uint cost[MAX_N][MAX_N]; /* Koszty krawędzi. */
uint64 opt[MAX_N][MAX_N]; /* Najniższe koszty triangulacji. */

uint64 triangle(const uint &a, const uint &b, const uint &c) {
	return cost[a][b] + cost[b][c] + cost[c][a];
}

void eval(const uint &a, const uint &b)
{
	opt[a][b] = +INF;
	for (uint k = a + 1; k < b; k++) {
		opt[a][b] = std::min(
			opt[a][b],
			opt[a][k] + opt[k][b] + cost[a][b]
		);
	}
}


void fill(const uint &n)
{
	for (uint i = 0; i < n; i++) {
		opt[i][i] = 0;
		opt[i][(i + 1) % n] = cost[i][(i + 1) % n];
	}

	/* Liczby dobierane na pałę. */
	for (uint l = 3; l <= n; l++) {
		for (uint i = 0; i < n - l + 1; i++) {
			eval(i, i + l - 1);
		}
	}
}

/* Wersja rekurencyjna powyższego, do odpluskiwania. */
uint64 rec(const uint &a, const uint &b)
{
	if (a == b) {
		return 0;
	}

	if (a == b - 1) {
		return cost[a][b];
	}

	uint64 result = +INF;
	for (uint k = a + 1; k < b; k++) {
		result = std::min(result, rec(a, k) + rec(k, b) + cost[a][b]);
	}

	return result;
}

int main()
{
	/* Wczytywańsko danych. */
	uint n;
	scanf("%u", &n);

	for (uint i = n - 1; i > 0; i--) {
		for (uint j = 0; j < i; j++) {
			/* Stałe ze względu na czytelność, kompilator zrobi swoje. */
			const uint a = n - 1 - i;
			const uint b = a + j + 1;

			int c;
			scanf("%u", &c);

			cost[a][b] = cost[b][a] = c;
		}
	}

	fill(n);

	printf("%llu\n", opt[0][n - 1]);

	return 0;
}
