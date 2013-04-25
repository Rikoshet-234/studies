#include <cstdio>
#include <algorithm>

typedef unsigned int uint;
typedef unsigned long long uint64;

#define MAX_NM 1000
#define INF (1u * 'I' * 'N' * 'F' * 'T' * 'Y')

uint lea[MAX_NM + 2][MAX_NM + 2]; /* Opis stokrotek na polanie. */
uint64 bot[MAX_NM + 2], top[MAX_NM + 2]; /* Najkrótsze ścieżki "dolne" i "górne". */
uint64 dp[MAX_NM + 2], d[MAX_NM + 2][MAX_NM + 2];

void fill(const uint &n, const uint &m)
{
	/* Ustawiamy strażników. */
	for (uint k = 0; k <= m + 1; k++) {
		lea[0][k] = lea[n + 1][k] = +INF;
		d[0][k] = d[n + 1][k] = +INF;
	}

	/* Ustalamy początkowe wartości. */
	for (uint w = 1; w <= n; w++) {
		d[w][1] = lea[w][1];
	}

	/* Resztę wyliczamy rekurencyjnie. */
	for (uint k = 2; k <= m; k++) {
		bot[0] = bot[n + 1] = top[0] = top[n + 1] = +INF;

		for (uint w = 1; w <= n; w++) {
			dp[w] = std::min(
				d[w][k - 1],
				std::min(d[w - 1][k - 1], d[w + 1][k - 1])
			);
		}

		for (uint w = 1; w <= n; w++) {
			bot[w] = lea[w - 1][k] + std::min(bot[w - 1], dp[w - 1]);
		}

		for (uint w = n; w >= 1; w--) {
			top[w] = lea[w + 1][k] + std::min(top[w + 1], dp[w + 1]);
		}
		for (uint w = 1; w <= n; w++) {
			d[w][k] = lea[w][k] + std::min(
				dp[w],
				std::min(bot[w], top[w])
			);
		}
	}
}

int main()
{
	uint n, m;
	scanf("%u %u", &n, &m);

	for (uint y = 1; y <= n; y++) {
		for (uint x = 1; x <= m; x++) {
			scanf("%u", &lea[y][x]);
		}
	}
	fill(n, m);

	uint64 dm = (uint64)(+INF) * (uint64)(+INF);
	for (uint w = 1; w <= n; w++) {
		dm = std::min(dm, d[w][m]);
	}
	printf("%llu\n", dm);

	return 0;
}
