#include <cstdio>
#include <algorithm>

typedef unsigned int uint;
typedef unsigned long long uint64;

#define MAX_N 1000000

uint perm[MAX_N];
uint sorted[MAX_N];

uint64 inversions(uint *perm, uint *sorted, const size_t &count)
{
	uint64 result = 0;

	/* Baza rekursji. */
	if (count == 0) {
		return result;
	}

	if (count == 1) {
		sorted[0] = perm[0];
		return result;
	}
	
	/* Dziel i rządź. */
	const size_t lsize = count / 2;
	const size_t rsize = count / 2 + count % 2;
	uint *lsorted = sorted;
	uint *rsorted = sorted + lsize;
	const uint64 lres = inversions(perm, lsorted, lsize);
	const uint64 rres = inversions(perm + lsize, rsorted, rsize);

	/* Liczba inwersji dla podproblemów. */
	result += lres + rres;
	
	/* Liczba inwersji "między" problemami. */
	for (uint i = 0; i < lsize; i++) {
		const uint *ub = std::upper_bound(
			sorted + lsize, sorted + lsize + rsize, perm[i]
		);
		result += ub - rsorted;
	}

	std::merge(
		lsorted, lsorted + lsize,
		rsorted, rsorted + rsize,
		perm
	);

	for (uint i = 0; i < count; i++) {
		sorted[i] = perm[i];
	}

	return result;
}

int main()
{
	uint n;
	scanf("%u", &n);

	for (uint i = 0; i < n; i++) {
		scanf("%u", &perm[i]);
	}

	printf("%llu\n", inversions(perm, sorted, n));

	return 0;
}
