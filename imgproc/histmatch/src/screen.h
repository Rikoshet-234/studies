#ifndef FILE_SCREEN_H
#define FILE_SCREEN_H


#include <stddef.h>
#include <stdint.h>

#include <SDL.h>

#include "hist.h"


#define HIST_HEIGHT 100
#define HIST_WIDTH 256


typedef struct {
	SDL_Surface *image;
	SDL_Surface *prehists[3];
	hist_t hists[3];
} screen_t;


void screen_init(screen_t *screen, SDL_Surface *image);
void screen_free(screen_t *screen);


#endif
