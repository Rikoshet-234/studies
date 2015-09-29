#include <stdbool.h>
#include <unistd.h>

#include "median.h"


void median_init(median_t *median)
{
	for (size_t i = 0; i < MEDIAN_HIST_SIZE; i++) {
		median->hist[i] = 0;
	}
	median->count = 0;
}


void median_free(median_t *median)
{
	(void)median;
}


uint8_t median_eval(const median_t *median)
{
	size_t accum = 0;
	for (size_t i = 0; i < MEDIAN_HIST_SIZE; i++) {
		accum += median->hist[i];
		if (accum >= (median->count + 1) / 2) {
			return i;
		}
	}

	return 0;
}


#define FRAME_LEFT_TO_RIGHT 0
#define FRAME_RIGHT_TO_LEFT 1


typedef struct {
	size_t width, height;
	size_t x, y;

	const texture_t *tex;
	median_t *median;
} frame_t;


static void frame_init(frame_t *frame, size_t width, size_t height,
                       const texture_t *tex, median_t *median);
static void frame_free(frame_t *frame);

static void frame_put_median_at(frame_t *frame, ssize_t x, ssize_t y);
static void frame_pop_median_at(frame_t *frame, ssize_t x, ssize_t y);
static void frame_put_median_row(frame_t *frame, ssize_t y);
static void frame_pop_median_row(frame_t *frame, ssize_t y);
static void frame_put_median_col(frame_t *frame, ssize_t x);
static void frame_pop_median_col(frame_t *frame, ssize_t x);
static bool frame_translate(frame_t *frame,
                            void *ctx, void (* callback)(void *, float));


static inline uint8_t frame_dir(const frame_t *frame)
{
	return frame->y % 2;
}


static inline ssize_t frame_xlb(const frame_t *frame)
{
	return -(ssize_t)frame->width / 2;
}


static inline ssize_t frame_xub(const frame_t *frame)
{
	return +(ssize_t)frame->width / 2 - ((ssize_t)(frame->width + 1) % 2);
}


static inline ssize_t frame_ylb(const frame_t *frame)
{
	return -(ssize_t)frame->height / 2;
}


static inline ssize_t frame_yub(const frame_t *frame)
{
	return +(ssize_t)frame->height / 2 - ((ssize_t)(frame->height + 1) % 2);
}


static void frame_init(frame_t *frame, size_t width, size_t height,
                       const texture_t *tex, median_t *median)
{
	frame->width = width;
	frame->height = height;
	frame->x = frame->y = 0;

	frame->tex = tex;
	frame->median = median;

	for (ssize_t y = frame_ylb(frame); y <= frame_yub(frame); y++) {
		frame_put_median_row(frame, y);
	}
}


static void frame_free(frame_t *frame)
{
	(void)frame;
}


static inline void frame_put_median_at(frame_t *frame, ssize_t x, ssize_t y)
{
	median_put(frame->median, texture_pixel_clamped(frame->tex, x, y));
}


static inline void frame_pop_median_at(frame_t *frame, ssize_t x, ssize_t y)
{
	median_pop(frame->median, texture_pixel_clamped(frame->tex, x, y));
}


static void frame_put_median_row(frame_t *frame, ssize_t y)
{
	for (ssize_t x = frame_xlb(frame); x <= frame_xub(frame); x++) {
		frame_put_median_at(frame, frame->x + x, y);
	}
}


static void frame_pop_median_row(frame_t *frame, ssize_t y)
{
	for (ssize_t x = frame_xlb(frame); x <= frame_xub(frame); x++) {
		frame_pop_median_at(frame, frame->x + x, y);
	}
}


static void frame_put_median_col(frame_t *frame, ssize_t x)
{
	for (ssize_t y = frame_ylb(frame); y <= frame_yub(frame); y++) {
		frame_put_median_at(frame, x, frame->y + y);
	}
}


static void frame_pop_median_col(frame_t *frame, ssize_t x)
{
	for (ssize_t y = frame_ylb(frame); y <= frame_yub(frame); y++) {
		frame_pop_median_at(frame, x, frame->y + y);
	}
}


static bool frame_translate(frame_t *frame,
                            void *ctx, void (* callback)(void *, float))
{
	const texture_t *tex = frame->tex;

	switch (frame_dir(frame)) {
	case FRAME_LEFT_TO_RIGHT:
		if (frame->x < tex->width - 1) {
			frame_pop_median_col(frame, frame->x + frame_xlb(frame));
			frame->x++;
			frame_put_median_col(frame, frame->x + frame_xub(frame));
		} else if (frame->y < tex->height - 1) {
			frame_pop_median_row(frame, frame->y + frame_ylb(frame));
			frame->y++;
			frame_put_median_row(frame, frame->y + frame_yub(frame));
		} else {
			return false;
		}
		break;
	case FRAME_RIGHT_TO_LEFT:
		if (frame->x != 0) {
			frame_pop_median_col(frame, frame->x + frame_xub(frame));
			frame->x--;
			frame_put_median_col(frame, frame->x + frame_xlb(frame));
		} else if (frame->y < tex->height - 1) {
			frame_pop_median_row(frame, frame->y + frame_ylb(frame));
			frame->y++;
			frame_put_median_row(frame, frame->y + frame_yub(frame));
		} else {
			return false;
		}
		break;
	}

	if (callback != NULL && frame->x == 0 && frame->y % 10 == 0) {
		callback(ctx, (float)frame->y / (float)(tex->height - 1));
	}
	return true;
}


void median_filter(const texture_t *input, texture_t *output,
                   size_t width, size_t height,
                   void *ctx, void (* callback)(void *, float))
{
	median_t median;
	median_init(&median);

	frame_t frame;
	frame_init(&frame, width, height, input, &median);

	do {
		texture_set_pixel(output, frame.x, frame.y, median_eval(&median));
	} while (frame_translate(&frame, ctx, callback));

	frame_free(&frame);
	median_free(&median);
}
