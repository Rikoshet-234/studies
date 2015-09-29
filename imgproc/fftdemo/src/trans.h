#ifndef FILE_TRANS_H
#define FILE_TRANS_H


#include <complex.h>
#include <fftw3.h>


void trans_init(size_t width, size_t height);

fftw_complex trans_0(fftw_complex v, size_t n, size_t m);
fftw_complex trans_1(fftw_complex v, size_t n, size_t m);
fftw_complex trans_2(fftw_complex v, size_t n, size_t m);
fftw_complex trans_3(fftw_complex v, size_t n, size_t m);
fftw_complex trans_4(fftw_complex v, size_t n, size_t m);
fftw_complex trans_5(fftw_complex v, size_t n, size_t m);
fftw_complex trans_6(fftw_complex v, size_t n, size_t m);
fftw_complex trans_7(fftw_complex v, size_t n, size_t m);
fftw_complex trans_8(fftw_complex v, size_t n, size_t m);


#endif
