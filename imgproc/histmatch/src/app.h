#ifndef FILE_APP_H
#define FILE_APP_H


#define APP_WIDTH 1152
#define APP_HEIGHT 648
#define APP_DELAY 20

#define MODE_SRC 0
#define MODE_PATT 1
#define MODE_COMB 2


#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include <SDL.h>
#include <SDL_image.h>

#include "hist.h"
#include "screen.h"


typedef struct {
	SDL_Surface *images[3];
	screen_t screens[3];
	uint8_t mode;

	SDL_Window *window;
	bool dirty;
	bool running;
} app_t;


bool app_init(app_t *app, const char *src, const char *patt);
void app_free(app_t *app);
void app_run(app_t *app);


#endif
