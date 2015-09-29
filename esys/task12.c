#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>


static inline bool valid(uint32_t v)
{
	return (v & (v - 1));
}


int main(void)
{
	uint32_t x;
	while (scanf("%u", &x) == 1) {
		if (valid(x)) {
			printf("%u\n", x);
		}
	}

	return 0;
}
