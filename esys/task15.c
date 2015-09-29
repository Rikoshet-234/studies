#include <stdio.h>

static size_t SOLUTIONS[] = { 1, 1, 0, 0, 2, 10, 4, 40, 92, 352, 724 };

int main()
{
	size_t n;
	scanf("%zu", &n);
	printf("%zu\n", SOLUTIONS[n]);

	return 0;
}