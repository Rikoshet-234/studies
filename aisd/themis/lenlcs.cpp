#include <cstdio>
#include <algorithm>

typedef unsigned int uint;

#define MAX_L 1000
char u[MAX_L + 1], v[MAX_L + 1];
uint lcs[MAX_L + 1][MAX_L + 1];

int main()
{
	uint t;
	scanf("%u", &t);

	for (; t > 0; t--) {
		/* Wczytywanie pierwszego napisu. */
		uint n;
		scanf("%u\n%s", &n, u);

		/* Wczytywanie drugiego napisu. */
		uint m;
		scanf("%u\n%s", &m, v);
		
		/* "Rekurencyjne" wyliczanie wartości LCS, przepadki brzegowe są zerami. */
		for (uint i = 1; i <= n; i++) {
			for (uint j = 1; j <= m; j++) {
				if (u[i - 1] == v[j - 1]) {
					lcs[i][j] = lcs[i - 1][j - 1] + 1;
				} else {
					lcs[i][j] = std::max(lcs[i - 1][j], lcs[i][j - 1]);
				}
			}
		}

		printf("%u\n", lcs[n][m]);
	}

	return 0;
}
