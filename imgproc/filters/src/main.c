#include <stdio.h>
#include <math.h>
#include <time.h>

#include <SDL.h>
#include <SDL_image.h>

#include "filter.h"
#include "task.h"
#include "utility.h"


#define IMG_DEFAULT_WIDTH 512
#define IMG_DEFAULT_HEIGHT 512


float wave(float x, float y)
{
	float d = hypot(x, y);
	float pi = atan(1.0f) * 4.0f;
	return 1.0f - (cos(exp(d) * d * pow(pi, pi)) + 1.0f) / 2.0f;
}


int main(int argc, char **argv)
{
	size_t n;
	if (argc < 3 || sscanf(argv[2], "%zu", &n) != 1) {
		fprintf(stderr, "ERROR: missing task name or filter size\n");
		return 1;
	}
	const char *task = argv[1];

	const char *out = NULL;
	if (argc >= 4) {
		out = argv[3];
	}

	const char *in = NULL;
	if (argc >= 5) {
		in = argv[4];
	}

	filter_t img;
	if (in != NULL) {
		if (!filter_load(&img, in)) {
			fprintf(stderr, "ERROR: failed to load \"%s\"\n", in);
			return 1;
		}
	} else {
		filter_init(&img, IMG_DEFAULT_WIDTH, IMG_DEFAULT_HEIGHT);
		filter_procedural(&img, wave);
	}

	if (strcmp(task, "a1") == 0) {
		task_a1(&img, n);
	} else if (strcmp(task, "a2") == 0) {
		task_a2(&img, n);
	} else if (strcmp(task, "a3") == 0) {
		task_a3(&img, n);
	} else if (strcmp(task, "a4") == 0) {
		task_a4(&img, n);
	} else if (strcmp(task, "a5") == 0) {
		task_a5(&img, n);
	} else if (strcmp(task, "b1") == 0) {
		task_b1(&img, n);
	} else if (strcmp(task, "b2") == 0) {
		task_b2(&img, n);
	} else if (strcmp(task, "b3") == 0) {
		task_b3(&img);
	} else {
		fprintf(stderr, "ERROR: unknown task \"%s\"\n", task);
		return 1;
	}

	if (out != NULL) {
		filter_save(&img, out);
	}

	filter_free(&img);

	return 0;
}
