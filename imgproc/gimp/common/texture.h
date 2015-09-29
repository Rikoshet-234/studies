#ifndef FILE_TEXTURE_H
#define FILE_TEXTURE_H


#include <stddef.h>
#include <stdint.h>
#include <unistd.h>

#include "utility.h"


typedef struct {
	uint8_t *data;
	size_t width, height;
	uint8_t stride;
} texture_t;


void texture_alloc(texture_t *tex, size_t width, size_t height, size_t bpp);
void texture_dealloc(texture_t *tex);

typedef uint8_t (* sampler_t)(const texture_t *, float, float);

uint8_t texture_sample_nn(const texture_t *tex, float u, float v);
uint8_t texture_sample_bilinear(const texture_t *tex, float u, float v);


static inline uint8_t texture_pixel(const texture_t *tex, size_t x, size_t y)
{
	return tex->data[(y * tex->width + x) * tex->stride];
}


static inline void texture_set_pixel(texture_t *tex,
                                     size_t x, size_t y, uint8_t value)
{
	tex->data[(y * tex->width + x) * tex->stride] = value;
}


static inline uint8_t texture_pixel_clamped(const texture_t *tex,
                                            ssize_t x, ssize_t y)
{
	size_t cx = CLAMP(x, 0, (ssize_t)tex->width - 1);
	size_t cy = CLAMP(y, 0, (ssize_t)tex->height - 1);
	return texture_pixel(tex, cx, cy);
}


#endif
