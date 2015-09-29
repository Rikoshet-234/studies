#ifndef FILE_APP_H
#define FILE_APP_H


#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include <SDL.h>
#include <SDL_image.h>

#include "screen.h"
#include "trans.h"


#define APP_DELAY 20
#define APP_SCALE_STEP 1.5
#define APP_BRIGHTNESS_STEP 2.0


typedef struct {
	const char *src, *dst;

	screen_t screen_init, screen_curr;

	SDL_Window *window;
	bool running;
} app_t;


bool app_init(app_t *app, const char *src, const char *dst);
void app_free(app_t *app);
void app_run(app_t *app);


#endif
