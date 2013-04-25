/*
 * Łukasz Hanuszczak
 * numer indeksu: 247952
 * grupa: KLO
 */

#include <cstdio>

typedef unsigned int uint;
typedef unsigned long long int ulong;

/*
 * Zwraca wartość wyrażenia  ((a^b) mod m)
 * używając alogrytmu szybkiego potęgowania
 */
uint powmod(const uint &a, const uint &b, const uint &m)
{
	if (b == 0) {
		return 1;
	}

	const ulong t = powmod(a, b / 2, m);
	const ulong t2 = (t * t) % m;
	return (b % 2 == 0) ? t2 : (t2 * a) % m;
}

int main()
{
	uint t;
	if (not scanf("%u", &t)) {
		return 1;
	}

	for (uint i = 0; i < t; i++) {
		uint a, b, m;
		if (not scanf("%u %u %u", &a, &b, &m)) {
			return 1;
		}
		printf("%u\n", powmod(a, b, m));
	}

	return 0;
}
