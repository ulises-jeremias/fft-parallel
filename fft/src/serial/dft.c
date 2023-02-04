#include <stdlib.h>
#include <math.h>
#include <complex.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

extern double complex complex_polar(double r, double theta);

double complex *
scic_dft_naive(double complex *x, size_t N)
{
    double complex *X = (double complex *) malloc(sizeof(double complex) * N);
    double im_k, im_base = -2.0 * M_PI / N;
    size_t k, n;

    for (k = 0; k < N; k++)
    {
        X[k] = 0.0 + 0.0 * I;
        im_k = im_base * k;

        for (n = 0; n < N; n++)
        {
            X[k] += x[n] * complex_polar(1.0, im_k * n);
        }
    }

    return X;
}
