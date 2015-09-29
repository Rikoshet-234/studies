#include "filter.h"

#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <complex.h>
#include <fftw3.h>


#define MAX(a, b) ((a) > (b) ? (a) : (b))


void filter_init(filter_t *filter, size_t height, size_t width)
{
	filter->data = (float *)malloc(sizeof(float) * height * width);
	filter->scale = 1.0f;
	filter->height = height;
	filter->width = width;
}


void filter_free(filter_t *filter)
{
	free(filter->data);
}


void filter_copy(const filter_t *src, filter_t *dst)
{
	memcpy(dst->data, src->data, sizeof(float) * src->width * src->height);
	dst->scale = src->scale;
}


static inline void filter_conv_at(const filter_t *src, const filter_t *ker,
                                  filter_t *out, size_t n, size_t m)
{
	float acc = 0;
	for (ssize_t y = filter_ylb(ker); y <= filter_yub(ker); y++) {
		for (ssize_t x = filter_xlb(ker); x <= filter_xub(ker); x++) {
			ssize_t sx = n - x;
			ssize_t sy = m - y;

			if (filter_xlb(src) <= sx && sx <= filter_xub(src) &&
			    filter_ylb(src) <= sy && sy <= filter_yub(src))
			{
				acc += FILTER_AT(src, sy, sx) * FILTER_AT(ker, y, x);
			}
		}
	}
	FILTER_AT(out, m, n) = acc;
}


void filter_conv(const filter_t *src, const filter_t *ker, filter_t *out)
{
	for (ssize_t m = filter_ylb(out); m <= filter_yub(out); m++) {
		for (ssize_t n = filter_xlb(out); n <= filter_xub(out); n++) {
			filter_conv_at(src, ker, out, m, n);
		}
	}
	out->scale = src->scale * ker->scale;
}


static void filter_forward_fft(const filter_t *src,
                               fftw_complex *buff, size_t height, size_t width)
{
	fftw_plan plan = fftw_plan_dft_2d(height, width, buff, buff,
	                                  FFTW_FORWARD, FFTW_ESTIMATE);
	for (size_t i = 0; i < height * width; i++) {
		buff[i] = 0.0;
	}
	for (size_t y = 0; y < src->height; y++) {
		for (size_t x = 0; x < src->width; x++) {
			buff[y * width + x] = src->data[y * src->width + x];
		}
	}
	fftw_execute(plan);

	fftw_destroy_plan(plan);
}


void filter_conv_fft(const filter_t *src, const filter_t *ker, filter_t *out)
{
	size_t width = src->width + ker->width - 1;
	size_t height = src->height + ker->height - 1;
	size_t size = width * height;

	fftw_complex *src_wav = fftw_malloc(sizeof(fftw_complex) * size);
	filter_forward_fft(src, src_wav, height, width);

	fftw_complex *ker_wav = fftw_malloc(sizeof(fftw_complex) * size);
	filter_forward_fft(ker, ker_wav, height, width);

	fftw_complex *out_wav = fftw_malloc(sizeof(fftw_complex) * size);
	fftw_plan out_plan = fftw_plan_dft_2d(height, width, out_wav, out_wav,
	                                      FFTW_BACKWARD, FFTW_ESTIMATE);

	for (size_t i = 0; i < size; i++) {
		out_wav[i] = src_wav[i] * ker_wav[i] / (float)size;
	}
	fftw_execute(out_plan);

	fftw_destroy_plan(out_plan);

	for (size_t y = 0; y < out->height; y++) {
		for (size_t x = 0; x < out->width; x++) {
			size_t yw = y + ker->height / 2;
			size_t xw = x + ker->width / 2;
			out->data[y * out->width + x] = creal(out_wav[yw * width + xw]);
		}
	}
	out->scale = src->scale * ker->scale;

	fftw_free(out_wav);
	fftw_free(ker_wav);
	fftw_free(src_wav);
}


void filter_box(filter_t *filter)
{
	for (size_t i = 0; i < filter->width * filter->height; i++) {
		filter->data[i] = 1.0f;
	}
	filter->scale = 1.0f / (float)(filter->width * filter->height);
}


void filter_binom_1d(filter_t *filter)
{
	size_t n = MAX(filter->width, filter->height);
	float *arr = malloc(sizeof(float) * 2 * (n + 1));

	for (size_t i = 0; i < n; i++) {
		arr[n + 1 + 0] = arr[n + 1 + i] = 1.0f;
		for (size_t k = 1; k < i; k++) {
			arr[n + 1 + k] = arr[k - 1] + arr[k];
		}
		for (size_t k = 0; k <= i; k++) {
			arr[k] = arr[n + 1 + k];
		}
	}

	memcpy(filter->data, arr, sizeof(float) * n);
	filter->scale = 1.0f / pow(2.0f, (float)n - 1.0f);

	free(arr);
}


void filter_binom_2d(filter_t *filter)
{
	size_t n = MAX(filter->width, filter->height);

	filter_t ver;
	filter_init(&ver, 1, n);
	filter_binom_1d(&ver);

	filter_t hor;
	filter_init(&hor, n, 1);
	filter_binom_1d(&hor);

	filter_conv(&ver, &hor, filter);

	filter_free(&hor);
	filter_free(&ver);
}


void filter_laplace_2d(filter_t *filter)
{
	for (size_t i = 0; i < 9; i++) {
		filter->data[i] = 1.0f;
	}
	filter->data[4] = -8.0f;
	filter->scale = 1.0f;
}


void filter_procedural(filter_t *filter, float (*f)(float, float))
{
	for (ssize_t m = filter_ylb(filter); m <= filter_yub(filter); m++) {
		for (ssize_t n = filter_xlb(filter); n <= filter_xub(filter); n++) {
			float x = (float)n / ((float)(filter->width) / 2.0f);
			float y = (float)m / ((float)(filter->height) / 2.0f);
			FILTER_AT(filter, m, n) = fmax(0.0f, fmin(f(x, y), 1.0f));
		}
	}
	filter->scale = 1.0f;
}
