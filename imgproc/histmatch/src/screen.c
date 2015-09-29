#include "screen.h"


static SDL_Surface *screen_prerender_hist(const hist_t *hist, uint32_t color)
{
	SDL_Surface *surface = SDL_CreateRGBSurface(
		0, HIST_WIDTH, HIST_HEIGHT, 32,
		0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF
	);
	uint32_t *pixels = (uint32_t *)surface->pixels;

	size_t max = 0;
	for (size_t x = 0; x < HIST_WIDTH; x++) {
		if (hist->values[x] > max) {
			max = hist->values[x];
		}
	}

	for (size_t x = 0; x < HIST_WIDTH; x++) {
		float factor = (float)hist->values[x] / (float)max;
		size_t height = (size_t)(factor * (float)HIST_HEIGHT + 0.5f);

		for (size_t y = 0; y < height; y++) {
			pixels[(HIST_HEIGHT - y - 1) * HIST_WIDTH + x] = color;
		}
		for (size_t y = height; y < HIST_HEIGHT; y++) {
			pixels[(HIST_HEIGHT - y - 1) * HIST_WIDTH + x] = 0x000000FF;
		}
	}

	return surface;
}


void screen_init(screen_t *screen, SDL_Surface *image)
{
	screen->image = image;

	size_t count = image->w * image->h;
	uint8_t bpp = image->format->BytesPerPixel;
	uint8_t *pixels = (uint8_t *)image->pixels;

	SDL_LockSurface(image);

	/* Red histogram. */
	hist_init(screen->hists + 0, pixels + 0, count, bpp);
	screen->prehists[0] = screen_prerender_hist(screen->hists + 0, 0xFF0000FF);

	/* Green histogram. */
	hist_init(screen->hists + 1, pixels + 1, count, bpp);
	screen->prehists[1] = screen_prerender_hist(screen->hists + 1, 0x00FF00FF);

	/* Blue histogram. */
	hist_init(screen->hists + 2, pixels + 2, count, bpp);
	screen->prehists[2] = screen_prerender_hist(screen->hists + 2, 0x0000FFFF);

	SDL_UnlockSurface(image);
}


void screen_free(screen_t *screen)
{
	for (uint8_t i = 0; i < 3; i++) {
		SDL_FreeSurface(screen->prehists[i]);
		hist_free(screen->hists + i);
	}
}
