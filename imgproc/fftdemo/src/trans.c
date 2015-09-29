#include "trans.h"

#include <stdbool.h>


static double trans_width;
static double trans_height;


void trans_init(size_t width, size_t height)
{
	trans_width = (double) width;
	trans_height = (double) height;
}



fftw_complex trans_0(fftw_complex v, size_t n, size_t m)
{
	return v;
}


fftw_complex trans_1(fftw_complex v, size_t n, size_t m)
{
	return creal(v);
}


fftw_complex trans_2(fftw_complex v, size_t n, size_t m)
{
	return cimag(v) * I;
}


fftw_complex trans_3(fftw_complex v, size_t n, size_t m)
{
	return cabs(v);
}


fftw_complex trans_4(fftw_complex v, size_t n, size_t m)
{
	return v / cabs(v);
}


fftw_complex trans_5(fftw_complex v, size_t n, size_t m)
{
	return creal(v) - cimag(v) * I;
}


fftw_complex trans_6(fftw_complex v, size_t n, size_t m)
{
	return 0.25 * creal(v) + cimag(v) * I;
}


fftw_complex trans_7(fftw_complex v, size_t n, size_t m)
{
	return v * (1.0 + (double)(n * n + m * m) / (trans_width * trans_height));
}


fftw_complex trans_8(fftw_complex v, size_t n, size_t m)
{
	double radius = 50.0 / 2.0;
	bool vert = (double)n <= radius || (double)n >= trans_width - radius;
	bool hor = (double)m <= radius || (double)m >= trans_height - radius;
	return vert && hor ? v : 0.0;
}
