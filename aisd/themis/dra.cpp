#include <cstdio>

typedef unsigned int uint;

#define MAX_N 1000
#define MAX_K (3 * MAX_N + 1)
#define M 500000003
uint s[MAX_N + 1][MAX_K + 1]; /* Ilość 1-drzew. */
uint d[MAX_N + 1][MAX_K + 1]; /* Ilość 2-drzew. */
char top[MAX_N + 1], mid[MAX_N + 2], bot[MAX_N + 1];

inline void eval(uint n, uint k)
{
	const uint t = top[n - 1] - '0', m = mid[n] - '0', b = bot[n - 1] - '0';

	switch (t << 2 | m << 1 | b << 0) {
	case 0:
		/*
		 *  ###
		 *    #
		 *  ###
		 */
		s[n][k] = 3 * s[n - 1][k] + d[n - 1][k];
		d[n][k] = 2 * s[n - 1][k] + d[n - 1][k];
		break;
	case 1: case 4:
		/*
		 * ###   ooo
		 *   #     #
		 * ooo   ###
		 */
		s[n][k] = s[n - 1][k];
		d[n][k] = s[n - 1][k];
		if (k >= 1) {
			s[n][k] += 2 * s[n - 1][k - 1] + d[n - 1][k - 1];
			d[n][k] += s[n - 1][k - 1] + d[n - 1][k - 1];
		}
		break;
	case 2:
		/*
		 * ###
		 *   o
		 * ###
		 */
		s[n][k] = s[n - 1][k];
		d[n][k] = 2 * s[n - 1][k] + d[n - 1][k];
		if (k >= 1) {
			s[n][k] += 2 * s[n - 1][k - 1] + d[n - 1][k - 1];
		}
		break;
	case 3: case 6:
		/*
		 * ###   ooo
		 *   o     o
		 * ooo   ###
		 */
		s[n][k] = 0;
		d[n][k] = s[n - 1][k];
		if (k >= 1) {
			s[n][k] += 2 * s[n - 1][k - 1];
			d[n][k] += s[n - 1][k - 1] + d[n - 1][k - 1];
		}
		if (k >= 2) {
			s[n][k] += s[n - 1][k - 2] + d[n - 1][k - 2];
		}
		break;
	case 5:
		/*
		 * ooo
		 *   #
		 * ooo
		 */
		s[n][k] = 0;
		d[n][k] = 0;
		if (k >= 1) {
			s[n][k] += 2 * s[n - 1][k - 1];
			d[n][k] += 2 * s[n - 1][k - 1];
		}
		if (k >= 2) {
			s[n][k] += s[n - 1][k - 2] + d[n - 1][k - 2];
			d[n][k] += d[n - 1][k - 2];
		}
		break;
	case 7:
		/*
		 * ooo
		 *   o
		 * ooo
		 */
		s[n][k] = 0;
		d[n][k] = 0;
		if (k >= 1) {
			d[n][k] += 2 * s[n - 1][k - 1];
		}
		if (k >= 2) {
			s[n][k] += 3 * s[n - 1][k - 2];
			d[n][k] += d[n - 1][k - 2];
		}
		if (k >= 3) {
			s[n][k] += d[n - 1][k - 3];
		}
		break;
	}

	s[n][k] %= M;
	d[n][k] %= M;
}

int main()
{
	uint t;
	scanf("%u", &t);

	for (; t > 0; t--) {
		uint n, k;
		scanf("%u %u", &n, &k);
		scanf("%s\n%s\n%s", top, mid, bot);

		for (uint i = 0; i <= k; i++) {
			s[0][i] = d[0][i] = 0;
		}

		d[0][0] = 1;
		if (mid[0] == '0') {
			s[0][0] = 1;
		} else {
			s[0][1] = 1;
		}

		for (uint ni = 1; ni <= n; ni++) {
			for (uint ki = 0; ki <= k; ki++) {
				eval(ni, ki);
			}
		}

		printf("%u\n", s[n][k]);
	}

	return 0;
}
