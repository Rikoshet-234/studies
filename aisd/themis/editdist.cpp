#include <cstdio>
#include <algorithm>

typedef unsigned int uint;

#define MAX_L 1000
char u[MAX_L + 1], v[MAX_L + 1];
uint dist[MAX_L + 1][MAX_L + 1];

int main()
{
	/* Wczytywanie pierwszego napisu. */
	uint n;
	scanf("%u %s", &n, u);

	/* Wczytywanie drugiego napisu. */
	uint m;
	scanf("%u %s", &m, v);

	/* Przypadki brzegowe. */
	for (uint i = 0; i <= n; i++) {
		dist[i][0] = i;
	}
	for (uint j = 0; j <= m; j++) {
		dist[0][j] = j;
	}

	/* "Rekurencyjne" wyliczanie wartości odległości edycyjnej. */
	for (uint i = 1; i <= n; i++) {
		for (uint j = 1; j <= m; j++) {
			if (u[i - 1] == v[j - 1]) {
				dist[i][j] = dist[i - 1][j - 1];
			} else {
				dist[i][j] = std::min(dist[i - 1][j], dist[i][j - 1]) + 1;
			}
		}
	}

	printf("%u\n", dist[n][m]);

	return 0;
}
