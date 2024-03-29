#ifndef SCIC_FFT_H
 #define SCIC_FFT_H

 #include <complex.h>

double complex * scic_dft_naive(double complex * x, size_t N);

double complex *scic_fft_pfa(double complex *x, size_t N, size_t N1, size_t N2);
double complex *scic_fft(double complex *x, size_t N, size_t N1, size_t N2);

double complex *scic_openmp_fft(double complex *input, size_t N, size_t N1, size_t N2, size_t num_threads);
double complex *scic_pthread_fft(double complex *input, size_t N, size_t N1, size_t N2, size_t num_threads);
 #endif
