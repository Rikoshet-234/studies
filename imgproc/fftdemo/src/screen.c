#include "screen.h"


#define A 0
#define R 1
#define G 2
#define B 3

#define PI (4.0 * atan(1.0))


static inline double map(double a1, double b1, double a2, double b2, double x)
{
	return a2 + (x - a1) * (b2 - a2) / (b1 - a1);
}


static inline double logscale(double s, double x)
{
	return s * log(1.0 + x);
}


static inline uint8_t screen_value(const screen_t *screen, size_t i)
{
	size_t size = screen->img.width * screen->img.height;
	double value = 0.0;

	switch (screen->mode) {
	case SCREEN_MODE_NORMAL:
		value = screen->img.data[i];
		break;
	case SCREEN_MODE_A:
		value = creal(screen->img.fourier[i] / (double)size);
		// value = map(-1.0, +1.0, 0.0, 256.0, value);
		value = logscale(screen->scale, value);
		break;
	case SCREEN_MODE_B:
		value = cimag(screen->img.fourier[i] / (double)size);
		// value = map(-1.0, +1.0, 0.0, 256.0, value);
		value = logscale(screen->scale, value);
		break;
	case SCREEN_MODE_R:
		value = cabs(screen->img.fourier[i]) / (double)size;
		// value = map(0.0, 1.0, 0.0, 256.0, value);
		value = logscale(screen->scale, value);
		break;
	case SCREEN_MODE_F:
		value = carg(screen->img.fourier[i]);
		value = map(-PI / 2.0, +PI / 2.0, 0.0, 256.0, value);
		break;
	}

	return (uint8_t)(fmax(0.0, fmin(255.0, screen->brightness * value + 0.5)));
}


static void screen_render(screen_t *screen)
{
	size_t size = screen->img.width * screen->img.height;
	uint8_t *pixels = screen->surface->pixels;

	for (size_t i = 0; i < size; i++) {
		uint8_t color = screen_value(screen, i);
		pixels[4 * i + R] = pixels[4 * i + G] = pixels[4 * i + B] = color;
		pixels[4 * i + A] = 0xFF;
	}
}


void screen_mode(screen_t *screen, uint8_t mode)
{
	screen->mode = mode;
	screen_render(screen);
}


void screen_scale(screen_t *screen, double scale)
{
	screen->scale = scale;
	screen_render(screen);
}


void screen_brightness(screen_t *screen, double brightness)
{
	screen->brightness = brightness;
	screen_render(screen);
}


void screen_init_surface(screen_t *screen, size_t width, size_t height)
{
	screen->surface = SDL_CreateRGBSurface(0, width, height, 32,
	                                       0xFF000000,
	                                       0x00FF0000, 0x0000FF00, 0x000000FF);	
}


void screen_free(screen_t *screen)
{
	img_free(&screen->img);
	SDL_FreeSurface(screen->surface);
}


bool screen_load(screen_t *screen, const char *path)
{
	SDL_Surface *surface = IMG_Load(path);
	if (surface == NULL) {
		fprintf(stderr, "failed to load image: %s\n", SDL_GetError());
		return false;
	}

	img_init(&screen->img, surface->pixels, surface->w, surface->h);
	screen_init_surface(screen, surface->w, surface->h);
	screen->scale = SCREEN_DEFAULT_SCALE;
	screen->brightness = SCREEN_DEFAULT_BRIGHTNESS;
	screen_mode(screen, SCREEN_MODE_NORMAL);

	SDL_FreeSurface(surface);

	return true;
}


bool screen_dump(const screen_t *screen, const char *path)
{
	if (SDL_SaveBMP(screen->surface, path) != 0) {
		fprintf(stderr, "failed to save image: %s\n", SDL_GetError());
		return false;
	}

	return true;
}


void screen_copy(const screen_t *src, screen_t *dst)
{
	img_copy(&src->img, &dst->img);
	screen_init_surface(dst, src->surface->w, src->surface->h);
	dst->scale = src->scale;
	dst->brightness = src->brightness;
	screen_mode(dst, src->mode);
}


void screen_transform(screen_t *screen, img_trans_t trans)
{
	img_transform(&screen->img, trans);
	screen_render(screen);
}
