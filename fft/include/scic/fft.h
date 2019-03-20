#ifndef SCIC_FFT_H
#define SCIC_FFT_H

#include <complex.h>

double complex *scic_dft_naive(double complex *x, size_t N);
double complex *scic_fft_pfa(double complex *x, size_t N, size_t N1, size_t N2);
double complex *scic_fft(double complex *x, size_t N, size_t N1, size_t N2);

#endif