#include <stdio.h>
#include <stdint.h>


static inline uint32_t intersperse(uint32_t v)
{
	uint32_t r = 0;
	for (size_t i = 0; i < sizeof(v) / 2; i++) {
		r <<= 2;
		r |= ((v & 1) << 1);
		v >>= 1;
	}

	return r;
}


static inline void sput_bin(uint32_t w, char *str)
{
	str[32] = '\0';
	for (size_t i = 0; i < 32; i++) {
		str[31 - i] = ((w >> i) & 1) ? '1' : '0';
	}
}


int main()
{
	char buff[33];
	for (uint32_t i = 0; i < 6; i++) {
		sput_bin(i, buff);
		printf("%s\n", buff);

		sput_bin(intersperse(i), buff);
		printf("%s\n", buff);

		printf("\n");
	}

	return 0;
}
