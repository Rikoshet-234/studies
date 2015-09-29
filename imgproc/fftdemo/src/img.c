#include "img.h"

#include <string.h>


#define A 0
#define R 1
#define G 2
#define B 3


static inline fftw_complex img_grayscale(const uint8_t *pixel)
{
	return 0.3 * pixel[R] + 0.6 * pixel[G] + 0.1 * pixel[B];
}


void img_fft(img_t *img, int8_t type)
{
	fftw_complex *src, *dst;
	switch (type) {
	case FFTW_FORWARD:
		src = img->data;
		dst = img->fourier;
		break;
	case FFTW_BACKWARD:
		src = img->fourier;
		dst = img->data;
		break;
	default:
		return;
	}

	fftw_plan plan = fftw_plan_dft_2d(img->width, img->height,
	                                  src, dst,
	                                  type, FFTW_ESTIMATE);
	fftw_execute(plan);
	fftw_destroy_plan(plan);
} 


void img_init(img_t *img, const uint8_t *pixels, size_t width, size_t height)
{
	size_t size = width * height;

	img->width = width;
	img->height = height;
	img->data = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * size);
	img->fourier = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * size);

	for (size_t i = 0; i < size; i++) {
		img->data[i] = img_grayscale(pixels + 3 * i);
	}
	img_fft(img, FFTW_FORWARD);
}


void img_free(img_t *img)
{
	fftw_free(img->fourier);
	fftw_free(img->data);
}


void img_copy(const img_t *src, img_t *dst)
{
	size_t size = src->width * src->height;

	dst->width = src->width;
	dst->height = src->height;
	dst->data = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * size);
	dst->fourier = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * size);

	memcpy(dst->data, src->data, sizeof(fftw_complex) * size);
	memcpy(dst->fourier, src->fourier, sizeof(fftw_complex) * size);
}


void img_transform(img_t *img, img_trans_t trans)
{
	size_t size = img->width * img->height;
	for (size_t m = 0; m < img->height; m++) {
		for (size_t n = 0; n < img->width; n++) {
			size_t i = m * img->width + n;
			img->fourier[i] = trans(img->fourier[i], n, m);
		}
	}

	img_fft(img, FFTW_BACKWARD);

	for (size_t i = 0; i < size; i++) {
		img->data[i] /= (double)size;
	}
}
