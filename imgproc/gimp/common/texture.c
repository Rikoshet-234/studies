#include <stdlib.h>
#include <math.h>

#include "texture.h"
#include "utility.h"


void texture_alloc(texture_t *tex, size_t width, size_t height, size_t bpp)
{
	tex->data = malloc(sizeof(uint8_t) * width * height * bpp);
	tex->width = width;
	tex->height = height;
	tex->stride = bpp;
}


void texture_dealloc(texture_t *tex)
{
	free(tex->data);
}


uint8_t texture_sample_nn(const texture_t *tex, float u, float v)
{
	ssize_t x = (ssize_t)round(u * (float)(tex->width - 1));
	ssize_t y = (ssize_t)round(v * (float)(tex->height - 1));
	return texture_pixel_clamped(tex, x, y);
}


uint8_t texture_sample_bilinear(const texture_t *tex, float u, float v)
{
	float cx = u * tex->width - 0.5f;
	float cy = v * tex->height - 0.5f;

	ssize_t x = (ssize_t)floor(cx);
	ssize_t y = (ssize_t)floor(cy);

	float dx = cx - (float)x;
	float dy = cy - (float)y;
	float dxp = 1.0f - dx;
	float dyp = 1.0f - dy;

	float s00 = dxp * dyp * (float)texture_pixel_clamped(tex, x, y);
	float s01 = dx * dyp * (float)texture_pixel_clamped(tex, x + 1, y);
	float s10 = dxp * dy * (float)texture_pixel_clamped(tex, x, y + 1);
	float s11 = dx * dy * (float)texture_pixel_clamped(tex, x + 1, y + 1);
	float s = s00 + s01 + s10 + s11;
	return (uint8_t)CLAMP(round(s), 0.0f, 255.0f);
}
