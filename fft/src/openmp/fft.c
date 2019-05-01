#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <omp.h>
#include <scic/fft.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

extern double complex complex_polar(double r, double theta);


/**
 * Implements the Cooley-Tukey FFT algorithm
 * 
 * @expects N1 and N2 must be relatively prime
 * @expects N1*N2 = N
 * 
 * @param double complex *, input
 * @param size_t N
 * @param size_t N1
 * @param size_t N2
 * @param size_t num_threads
 * 
 * @return double complex *
 * 
 */
double complex *
scic_openmp_fft(double complex *input, size_t N, size_t N1, size_t N2, size_t num_threads)
{
        double complex **columns, **rows, *output;
        size_t k1, k2;

        omp_set_num_threads(num_threads);
        // printf("%d, %d\n", omp_get_max_threads(), omp_get_num_threads());

        /* Allocate columnwise matrix */
        columns = (double complex**) malloc(sizeof(double complex*) * N1);

        for (k1 = 0; k1 < N1; k1++)
        {
                columns[k1] = (double complex*) malloc(sizeof(double complex) * N2);
        }

        /* Allocate rowwise matrix */
        rows = (double complex**) malloc(sizeof(double complex*) * N2);

        for (k2 = 0; k2 < N2; k2++)
        {
                rows[k2] = (double complex*) malloc(sizeof(double complex) * N1);
        }

        output = (double complex*) malloc(sizeof(double complex) * N);

        #pragma omp parallel
        {
                // printf("Thread: %d, CPU: %d\n", omp_get_thread_num(), sched_getcpu());

                /* Reshape input into N1 columns */
                #pragma omp for private(k2)
                for (k1 = 0; k1 < N1; k1++)
                {
                        for (k2 = 0; k2 < N2; k2++)
                        {
                                columns[k1][k2] = input[N1*k2 + k1];
                        }
                }

                /* Compute N1 DFTs of length N2 using naive method */
                #pragma omp for schedule(dynamic)
                for (k1 = 0; k1 < N1; k1++)
                {
                        columns[k1] = scic_dft_naive(columns[k1], N2);
                }

                /* Multiply by the twiddle factors exp(-2*pi*k1*k2*i/N) and transpose */
                #pragma omp for private(k2)
                for (k1 = 0; k1 < N1; k1++)
                {
                        for (k2 = 0; k2 < N2; k2++)
                        {
                                rows[k2][k1] = complex_polar(1, -2*M_PI*k1*k2/N) * columns[k1][k2];
                        }
                }

                /* Compute N2 DFTs of length N1 using naive method */
                #pragma omp for schedule(dynamic)
                for (k2 = 0; k2 < N2; k2++)
                {
                        rows[k2] = scic_dft_naive(rows[k2], N1);
                }

                /* Flatten into single output */

                #pragma omp for private(k2)
                for (k1 = 0; k1 < N1; k1++)
                {
                        for (k2 = 0; k2 < N2; k2++)
                        {
                                output[N2*k1 + k2] = rows[k2][k1];
                        }
                }
        }

        /* Free all alocated memory except output and input arrays */
        for (k1 = 0; k1 < N1; k1++)
        {
                free(columns[k1]);
        }

        for (k2 = 0; k2 < N2; k2++)
        {
                free(rows[k2]);
        }

        free(columns);
        free(rows);

        return output;
}
