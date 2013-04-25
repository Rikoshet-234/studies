/*
 * Łukasz Hanuszczak
 * numer indeksu: 247952
 * grupa: KLO
 */

#include <cstdio>

typedef unsigned int uint;

#define MAX_N 100000
uint h[MAX_N];
uint n, k;

bool fit(const uint &maxh)
{
	uint currk = k; /* Ile przegródek zostało do postawienia. */
	uint sum = 0; /* Suma na aktualnym przedziale. */
	
	for (uint i = 0; i < n; i++) {
		sum += h[i];
		if (sum > maxh) {
			/* Nie da się tego naprawić, przykrość. */
			if (h[i] > maxh) {
				return false;
			}

			/* Stawiamy przegródkę. */
			sum = h[i];
			if (currk == 0) {
				return false;
			} else {
				currk--;
			}
		}
	}

	/* Wszystko fajnie się rozstawiło, daliśmy radę. */
	return true;
}

uint binsearch(const uint &a, const uint &b)
{
	const uint size = b - a;

	/* Baza rekursji. */
	if (size == 0) {
		return a;
	}

	/* Idziemy w lewo albo w prawo. */
	const uint mid = a + size / 2;
	return fit(mid) ? binsearch(a, mid) : binsearch(mid + 1, b);
}

int main()
{
	scanf("%u %u", &n, &k);

	uint sum = 0;
	for (uint i = 0; i < n; i++) {
		scanf("%u", &h[i]);
		sum += h[i];
	}

	printf("%u\n", binsearch(0, sum));

	return 0;
}
