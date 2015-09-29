#ifndef FILE_FILTER_H
#define FILE_FILTER_H


#include <sys/types.h>
#include <stdbool.h>


typedef struct {
	float *data;
	float scale;
	size_t height, width;
} filter_t;


void filter_init(filter_t *filter, size_t height, size_t width);
void filter_free(filter_t *filter);
void filter_copy(const filter_t *src, filter_t *dst);
void filter_conv(const filter_t *src, const filter_t *ker, filter_t *out);
void filter_conv_fft(const filter_t *src, const filter_t *ker, filter_t *out);

void filter_box(filter_t *filter);
void filter_binom_1d(filter_t *filter);
void filter_binom_2d(filter_t *filter);
void filter_laplace_2d(filter_t *filter);
void filter_procedural(filter_t *filter, float (*f)(float, float));


static inline ssize_t filter_xlb(const filter_t *filter)
{
	return -((ssize_t)filter->width / 2);
}


static inline ssize_t filter_xub(const filter_t *filter)
{
	return +((ssize_t)filter->width / 2) - (filter->width + 1) % 2;;
}


static inline ssize_t filter_ylb(const filter_t *filter)
{
	return -((ssize_t)filter->height / 2);
}


static inline ssize_t filter_yub(const filter_t *filter)
{
	return +((ssize_t)filter->height / 2) - (filter->height + 1) % 2;;
}


static inline ssize_t filter_x(const filter_t *filter, ssize_t x)
{
	return x + (ssize_t)filter->width / 2;
}


static inline ssize_t filter_y(const filter_t *filter, ssize_t y)
{
	return y + (ssize_t)filter->height / 2;
}


#define FILTER_AT(f, m, n) \
	((f)->data[(filter_y(f, m)) * f->width + (filter_x(f, n))])


#endif
