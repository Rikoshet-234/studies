#include "app.h"


bool app_init(app_t *app, const char *src, const char *dst)
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		fprintf(stderr, "failed to initialize video: %s\n", SDL_GetError());
		return false;
	}

	app->src = src;
	app->dst = dst;

	if (!screen_load(&app->screen_init, src)) {
		return false;
	}
	screen_copy(&app->screen_init, &app->screen_curr);

	SDL_Window *window = SDL_CreateWindow(
		"FFTW Demo",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		app->screen_init.surface->w, app->screen_init.surface->h,
		SDL_WINDOW_SHOWN
	);
	if (window == NULL) {
		fprintf(stderr, "failed to create window: %s\n", SDL_GetError());
		return false;
	}

	trans_init(app->screen_init.img.width, app->screen_init.img.height);

	app->window = window;
	app->running = true;
	return true;
}


void app_free(app_t *app)
{
	screen_free(&app->screen_init);
	screen_free(&app->screen_curr);

	SDL_DestroyWindow(app->window);
	SDL_Quit();
}


static void app_dump(app_t *app)
{
	screen_dump(&app->screen_curr, app->dst);
}


static void app_transform(app_t *app, img_trans_t trans)
{
	uint8_t mode = app->screen_curr.mode;

	screen_free(&app->screen_curr);
	screen_copy(&app->screen_init, &app->screen_curr);
	screen_transform(&app->screen_curr, trans);
	screen_mode(&app->screen_curr, mode);
}


static void app_shift(app_t *app)
{
	uint8_t mode = app->screen_curr.mode;

	screen_free(&app->screen_curr);
	screen_copy(&app->screen_init, &app->screen_curr);

	img_t *img = &app->screen_curr.img;
	size_t width = img->width, height = img->height;

	for (size_t m = 0; m < height; m++) {
		for (size_t n = 0; n < width; n++) {
			if ((n + m) % 2 == 1) {
				img->data[m * width + n] = -img->data[m * width + n];
			}
		}
	}
	img_fft(img, FFTW_FORWARD);

	screen_mode(&app->screen_curr, mode);
}


static void app_handle_key(app_t *app, SDL_Keycode key)
{
	screen_t *screen = &app->screen_curr;

	switch (key) {
	case SDLK_s:
		app_dump(app);
		break;
	case SDLK_n:
		screen_mode(screen, SCREEN_MODE_NORMAL);
		break;
	case SDLK_a:
		screen_mode(screen, SCREEN_MODE_A);
		break;
	case SDLK_b:
		screen_mode(screen, SCREEN_MODE_B);
		break;
	case SDLK_r:
		screen_mode(screen, SCREEN_MODE_R);
		break;
	case SDLK_f:
		screen_mode(screen, SCREEN_MODE_F);
		break;
	case SDLK_UP:
		screen_scale(screen, screen->scale * APP_SCALE_STEP);
		break;
	case SDLK_DOWN:
		screen_scale(screen, screen->scale / APP_SCALE_STEP);
		break;
	case SDLK_RIGHT:
		screen_brightness(screen, screen->brightness * APP_BRIGHTNESS_STEP);
		break;
	case SDLK_LEFT:
		screen_brightness(screen, screen->brightness / APP_BRIGHTNESS_STEP);
		break;
	case SDLK_LSHIFT:
	case SDLK_RSHIFT:
		app_shift(app);
		break;
	case SDLK_0:
		app_transform(app, trans_0);
		break;
	case SDLK_1:
		app_transform(app, trans_1);
		break;
	case SDLK_2:
		app_transform(app, trans_2);
		break;
	case SDLK_3:
		app_transform(app, trans_3);
		break;
	case SDLK_4:
		app_transform(app, trans_4);
		break;
	case SDLK_5:
		app_transform(app, trans_5);
		break;
	case SDLK_6:
		app_transform(app, trans_6);
		break;
	case SDLK_7:
		app_transform(app, trans_7);
		break;
	case SDLK_8:
		app_transform(app, trans_8);
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
	SDL_BlitSurface(app->screen_curr.surface, NULL, surface, NULL);
	SDL_UpdateWindowSurface(app->window);
}


void app_run(app_t *app)
{
	while (app->running) {
		app_handle_events(app);
		app_draw(app);

		SDL_Delay(APP_DELAY);
	}
}


int main(int argc, char **argv)
{
	const char *src = argc >= 1 ? argv[1] : NULL;
	const char *dst = argc >= 2 ? argv[2] : NULL;

	app_t app;
	if (!app_init(&app, src, dst)) {
		return 1;
	}

	app_run(&app);

	app_free(&app);

	return 0;
}
