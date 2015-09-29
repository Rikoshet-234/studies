#ifndef FILE_MEDIAN_H
#define FILE_MEDIAN_H


#include <stddef.h>
#include <stdint.h>

#include "../common/texture.h"
#include "../common/utility.h"


#define MEDIAN_HIST_SIZE (1 << (sizeof(uint8_t) * 8))

typedef struct {
	size_t hist[MEDIAN_HIST_SIZE];
	size_t count;
} median_t;


void median_init(median_t *median);
void median_free(median_t *median);
uint8_t median_eval(const median_t *median);


static inline void median_put(median_t *median, uint8_t value)
{
	median->hist[value]++;
	median->count++;
}


static inline void median_pop(median_t *median, uint8_t value)
{
	median->hist[value]--;
	median->count--;
}


void median_filter(const texture_t *input, texture_t *output,
                   size_t width, size_t height,
                   void *ctx, void (* callback)(void *, float));


#endif
