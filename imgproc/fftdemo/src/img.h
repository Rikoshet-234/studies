#ifndef FILE_IMG_H
#define FILE_IMG_H


#include <stdint.h>
#include <stddef.h>

#include <complex.h>
#include <fftw3.h>


typedef struct {
	size_t width, height;
	fftw_complex *data;
	fftw_complex *fourier;
} img_t;

typedef fftw_complex (* img_trans_t)(fftw_complex, size_t, size_t);


void img_init(img_t *img, const uint8_t *pixels, size_t width, size_t height);
void img_free(img_t *img);
void img_copy(const img_t *src, img_t *dst);
void img_transform(img_t *img, img_trans_t trans);
void img_fft(img_t *img, int8_t type);


#endif
