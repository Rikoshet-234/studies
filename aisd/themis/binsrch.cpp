/*
 * Łukasz Hanuszczak
 * numer indeksu: 247952
 * grupa: KLO
 */

#include <cstdio>

typedef unsigned int uint;

/*
 * Używając wyszukiwania binarnego zwraca wskaźnik
 * do najwcześniejszego elementu zadanej posortowanej
 * tablicy takiego, że każdy następny jest od niego
 * niemniejszy.
 */
uint *lookup(uint *ptr, uint size, uint x)
{
	if (size == 1) {
		if (ptr[0] >= x) {
			return ptr;
		} else {
			return ptr + 1;
		}
	}

	const uint mid = size / 2;
	if (x <= ptr[mid]) {
		return lookup(ptr, mid, x);
	} else {
		return lookup(ptr + mid, mid + (size % 2), x);
	}
}

#define MAX_N 1000000
uint arr[MAX_N + 1];

int main()
{
	uint n;
	scanf("%u", &n);

	for (uint i = 0; i < n; i++) {
		scanf("%u", &arr[i]);
	}

	uint m;
	scanf("%u", &m);

	for (uint i = 0; i < n; i++) {
		uint x;
		scanf("%u", &x);
		printf("%u ", n - (uint)(lookup(arr, n, x) - arr));
	}
	printf("\n");

	return 0;
}
