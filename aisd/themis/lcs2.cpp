#include <cstdio>
#include <cstring>
#include <algorithm>

typedef unsigned int uint;

#define MAX_L 1000
char u[MAX_L + 1], v[MAX_L + 1], w[MAX_L + 1];
uint lcs[MAX_L + 1][MAX_L + 1];

int main()
{
	scanf("%s", u);
	const uint n = strlen(u);
	
	scanf("%s", v);
	const uint m = strlen(v);

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

	/* Zczytanie najdłuższego podsłowa "od tyłu". */
	w[lcs[n][m]] = '\0';
	uint ci = n, cj = m;
	for (uint k = lcs[n][m]; k > 0;) {
		if (u[ci - 1] == v[cj - 1]) {
			w[k - 1] = u[ci - 1];
			ci--;
			cj--;
			k--;
		} else if (lcs[ci - 1][cj] >= lcs[ci][cj - 1]) {
			ci--;
		} else {
			cj--;
		}
	}

	printf("%s\n", w);

	return 0;
}
