#include "utility.h"

#include <SDL.h>
#include <SDL_image.h>


bool filter_load(filter_t *filter, const char *path)
{
	SDL_Surface *surf = IMG_Load(path);
	if (surf == NULL) {
		return false;
	}

	size_t width = surf->w;
	size_t height = surf->h;
	uint8_t bpp = surf->format->BytesPerPixel;
	uint8_t *pixels = surf->pixels;

	filter_init(filter, height, width);
	for (size_t i = 0; i < width * height; i++) {
		uint8_t *pixel = pixels + i * bpp;
		float r = pixel[0] / 256.0f;
		float g = pixel[1] / 256.0f;
		float b = pixel[2] / 256.0f;
		filter->data[i] = (r + g + b) / 3.0f;
	}
	filter->scale = 1.0f;

	SDL_FreeSurface(surf);
	return true;
}


void filter_save(const filter_t *filter, const char *path)
{
	size_t width = filter->width;
	size_t height = filter->height;
	float scale = filter->scale;

	SDL_Surface *surf =	SDL_CreateRGBSurface(0, width, height, 32,
	                                         0x00FF0000,
	                                         0x0000FF00,
	                                         0x000000FF,
	                                         0xFF000000);

	for (size_t i = 0; i < width * height; i++) {
		uint8_t *pixel = (uint8_t *)surf->pixels + i * 4;
		float value = filter->data[i];
		pixel[0] = pixel[1] = pixel[2] = (uint8_t)(value * scale * 256.0f);
	}

	SDL_SaveBMP(surf, path);
	SDL_FreeSurface(surf);
}
