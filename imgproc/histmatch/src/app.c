#include "app.h"


static inline SDL_Surface *sdl_surface_copy(SDL_Surface *surf)
{
	return SDL_ConvertSurface(surf, surf->format, 0);
}


static inline void app_set_mode(app_t *app, uint8_t mode)
{
	app->mode = mode;
	app->dirty = true;
}


static void app_init_comb(app_t *app)
{
	SDL_Surface *image = sdl_surface_copy(app->images[MODE_SRC]);
	
	size_t count = image->w * image->h;
	size_t bpp = image->format->BytesPerPixel;
	uint8_t *pixels = (uint8_t *)image->pixels;

	if (app->images[MODE_PATT] != NULL) {
		hist_match(&app->screens[MODE_PATT].hists[0], pixels + 0, count, bpp);
		hist_match(&app->screens[MODE_PATT].hists[1], pixels + 1, count, bpp);
		hist_match(&app->screens[MODE_PATT].hists[2], pixels + 2, count, bpp);
	} else {
		hist_t hist;
		hist_const(&hist);

		hist_match(&hist, pixels + 0, count, bpp);
		hist_match(&hist, pixels + 1, count, bpp);
		hist_match(&hist, pixels + 2, count, bpp);

		hist_free(&hist);
	}

	app->images[MODE_COMB] = image;
	screen_init(app->screens + MODE_COMB, app->images[MODE_COMB]);
}


bool app_init(app_t *app, const char *src, const char *patt)
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		fprintf(stderr, "failed to initialize video: %s\n", SDL_GetError());
		return false;
	}

	SDL_Window *window = SDL_CreateWindow(
		"Histogram matching",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		APP_WIDTH, APP_HEIGHT,
		SDL_WINDOW_SHOWN
	);
	if (window == NULL) {
		fprintf(stderr, "failed to create window: %s\n", SDL_GetError());
		return false;
	}

	app->images[MODE_SRC] = IMG_Load(src);
	screen_init(app->screens + MODE_SRC, app->images[MODE_SRC]);

	if (patt != NULL) {
		app->images[MODE_PATT] = IMG_Load(patt);
		screen_init(app->screens + MODE_PATT, app->images[MODE_PATT]);
	} else {
		app->images[MODE_PATT] = NULL;
	}

	app_init_comb(app);
	app_set_mode(app, MODE_SRC);

	app->window = window;
	app->running = true;
	return true;
}


void app_free(app_t *app)
{
	screen_free(app->screens + MODE_SRC);
	SDL_FreeSurface(app->images[MODE_SRC]);

	if (app->images[MODE_PATT] != NULL) {
		screen_free(app->screens + MODE_PATT);
		SDL_FreeSurface(app->images[MODE_PATT]);
	}

	screen_free(app->screens + MODE_COMB);
	SDL_FreeSurface(app->images[MODE_COMB]);

	SDL_DestroyWindow(app->window);
	SDL_Quit();
}


static void app_handle_key(app_t *app, SDL_Keycode key)
{
	switch (key) {
	case SDLK_1:
		app_set_mode(app, MODE_SRC);
		break;
	case SDLK_2:
		if (app->images[MODE_PATT] != NULL) {
			app_set_mode(app, MODE_PATT);
		}
		break;
	case SDLK_3:
		app_set_mode(app, MODE_COMB);
		break;
	case SDLK_ESCAPE:
		app->running = false;
		break;
	}
}


static void app_handle_events(app_t *app)
{
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			app->running = false;
			break;
		case SDL_KEYDOWN:
			app_handle_key(app, event.key.keysym.sym);
			break;
		}
	}
}


static void app_draw(app_t *app)
{
	SDL_Surface *surface = SDL_GetWindowSurface(app->window);
	SDL_FillRect(surface, NULL, 0xFF000000);

	SDL_Surface *image = app->images[app->mode];

	size_t canv_width = APP_WIDTH - HIST_WIDTH;
	size_t canv_height = APP_HEIGHT;
	float scale = fmin((float)canv_width / (float)image->w,
	                   (float)canv_height / (float)image->h);

	SDL_Rect rect;
	rect.x = 0;
	rect.y = 0;
	rect.w = (size_t)(image->w * scale + 0.5f);
	rect.h = (size_t)(image->h * scale + 0.5f);

	SDL_BlitScaled(app->images[app->mode], NULL, surface, &rect);

	for (size_t i = 0; i < 3; i++) {
		SDL_Rect rect;
		rect.x = APP_WIDTH - HIST_WIDTH;
		rect.y = HIST_HEIGHT * i;
		rect.w = HIST_WIDTH;
		rect.h = HIST_HEIGHT;

		SDL_Surface *prehist = app->screens[app->mode].prehists[i];
		SDL_BlitSurface(prehist, NULL, surface, &rect);
		
	}
	SDL_UpdateWindowSurface(app->window);
}


void app_run(app_t *app)
{
	while (app->running) {
		app_handle_events(app);
		if (app->dirty) {
			app_draw(app);
			app->dirty = false;
		}
		SDL_Delay(APP_DELAY);
	}
}


int main(int argc, char **argv)
{
	const char *src = argc >= 1 ? argv[1] : NULL;
	const char *patt = argc >= 2 ? argv[2] : NULL;

	app_t app;
	if (!app_init(&app, src, patt)) {
		return 1;
	}

	app_run(&app);

	app_free(&app);
	return 0;
}
