/*
 * Łukasz Hanuszczak
 * numer indeksu: 247952
 * grupa: KLO
 */

#include <cstdio>
#include <cmath>

int main()
{
	int a, b;
	if (not scanf("%d %d", &a, &b)) {
		return 1;
	}

	for (int i = a; i <= b; i++) {
		if (std::abs(i % 2) == 1) {
			printf("%d ", i);
		}
	}
	printf("\n");

	return 0;
}
