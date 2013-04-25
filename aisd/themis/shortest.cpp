/*
 * Łukasz Hanuszczak
 * numer indeksu: 247952
 * grupa: KLO
 */

#include <cstdio>
#include <cmath>
#include <algorithm>

typedef unsigned int uint;
typedef long long int64;

struct point {
	int x, y;
};

bool cmpx(const point &a, const point &b) {
	return a.x < b.x;
}

bool cmpy(const point &a, const point &b) {
	return a.y < b.y;
}

inline int64 dst2(const point &a, const point &b) {
	const int64 x = a.x - b.x;
	const int64 y = a.y - b.y;
	return x * x + y * y;
}

#define INF 4611686018427387904
#define MAX_N 100000
point xs[MAX_N], ts[MAX_N];

int64 shortest(const point *ps, const uint &n) {
	if (n <= 2) {
		return (n == 2) ? dst2(ps[0], ps[1]) : +INF;
	}

	int64 d = std::min(shortest(ps, n / 2), shortest(ps + n / 2, n / 2 + n % 2));

	/* Prosta względem której dzielimy na dwa zbiory. */
	const int l = ps[n / 2].x;

	/* Do tablicy `ts` przerzucamy punkty, które są niedaleko prostej l. */
	uint nt = 0;
	for (uint i = 0; i < n; i++) {
		const int64 dx = l - ps[i].x;
		if (dx * dx < d) {
			ts[nt++] = ps[i];
		}
	}

	if (nt == 0) {
		return d;
	}

	std::sort(ts, ts + nt, cmpy);

	/* Sprawdzamy czy w centrum nie ma mniejszego punktu. */
	for (uint i = 0; i < nt - 1; i++) {
		const uint k = std::min(7u, nt - i - 1);
		for (uint j = 0; j < k; j++) {
			const int64 cd = dst2(ts[i], ts[i + j + 1]);
			if (cd < d) {
				d = cd;
			}
		}
	}

	return d;
}

int main()
{
	uint n;
	scanf("%u", &n);

	for (uint i = 0; i < n; i++) {
		int x, y;
		scanf("%d %d", &x, &y);
		xs[i].x = x;
		xs[i].y = y;
	}

	std::sort(xs, xs + n, cmpx);

	printf("%0.5lf\n", sqrt(shortest(xs, n)));
}
