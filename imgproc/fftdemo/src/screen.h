#ifndef FILE_SCREEN_H
#define FILE_SCREEN_H


#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include <SDL.h>
#include <SDL_image.h>

#include "img.h"


#define SCREEN_MODE_NORMAL 0
#define SCREEN_MODE_A 1
#define SCREEN_MODE_B 2
#define SCREEN_MODE_R 3
#define SCREEN_MODE_F 4

#define SCREEN_DEFAULT_SCALE 1024.0
#define SCREEN_DEFAULT_BRIGHTNESS 1.0

typedef struct {
	SDL_Surface *surface;
	img_t img;
	uint8_t mode;
	double scale;
	double brightness;
} screen_t;


void screen_free(screen_t *screen);
bool screen_load(screen_t *screen, const char *path);
bool screen_dump(const screen_t *screen, const char *path);
void screen_copy(const screen_t *src, screen_t *dst);
void screen_transform(screen_t *screen, img_trans_t trans);

void screen_mode(screen_t *screen, uint8_t mode);
void screen_scale(screen_t *screen, double scale);
void screen_brightness(screen_t *screen, double brightness);

#endif
