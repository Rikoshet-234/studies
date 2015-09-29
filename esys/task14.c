#include <stdio.h>
#include <stdint.h>


static inline uint32_t word_rev1(uint32_t w)
{
	uint32_t r = w;
	for (uint32_t i = 0; i < 32; i++) {
		r <<= 1;
		r |= w & 1;
		w >>= 1;
	}

	return r;
}


static inline uint8_t byte_rev2(uint8_t b)
{
	return ((b * 0x80200802ULL) & 0x0884422110ULL) * 0x0101010101ULL >> 32;
}


static inline uint32_t word_rev2(uint32_t w)
{
	uint8_t *b = (uint8_t *)(&w);
	return (byte_rev2(b[3]) << (0 * 8)) |
	       (byte_rev2(b[2]) << (1 * 8)) |
	       (byte_rev2(b[1]) << (2 * 8)) |
	       (byte_rev2(b[0]) << (3 * 8));
}


static inline uint32_t byte_rev3(uint32_t b)
{
	return (b * 0x0202020202ULL & 0x010884422010ULL) % 1023;
}


static inline uint32_t word_rev3(uint32_t w)
{
	uint8_t *b = (uint8_t *)(&w);
	return (byte_rev3(b[3]) << (0 * 8)) |
	       (byte_rev3(b[2]) << (1 * 8)) |
	       (byte_rev3(b[1]) << (2 * 8)) |
	       (byte_rev3(b[0]) << (3 * 8));	
}


#define N 1000000000
#define REV word_rev3

static inline void sput_bin(uint32_t w, char *str)
{
	str[32] = '\0';
	for (size_t i = 0; i < 32; i++) {
		str[31 - i] = ((w >> i) & 1) ? '1' : '0';
	}
}


int main()
{
	uint32_t s = 0;
	for (size_t i = 0; i < N; i++) {
		s += REV(i);
	}

	printf("%u\n", s);
}


/*
 * N = 1 000 000 000
 *
 *           | word_rev1 | word_rev2 | word_rev3
 * gcc -O2   |   35.159s |    6.267s |   12.522s
 * clang -O2 |   34.332s |    6.228s |   13.578s
 */
