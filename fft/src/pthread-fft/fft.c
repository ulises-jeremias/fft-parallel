#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <pthread.h>
#include <scic/fft.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

typedef struct scic_pthread_arg
{
        double complex **columns;
        double complex **rows;
        double complex *output;
} scic_pthread_arg_t;

extern double complex complex_polar(double r, double theta);

void *
__scic_pthread_fft_compute(void *arg)
{
        scic_pthread_arg_t *arg_struct = (scic_pthread_arg_t *) arg;

        return NULL;
}

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
scic_pthread_fft(double complex *input, size_t N, size_t N1, size_t N2, size_t num_threads)
{      
        double complex **columns, **rows, *output;
        size_t i, k1, k2;
        pthread_t tid[num_threads];

        scic_pthread_arg_t arg = (scic_pthread_arg_t) {
              .columns=columns,
              .rows=rows,
              .output=output  
        };
  
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

        /* Allocate output array */
        output = (double complex*) malloc(sizeof(double complex) * N);

        for (i = 0; i < num_threads; i++)
        {
                pthread_create(&tid[i], NULL, &__scic_pthread_fft_compute, (void *) &arg);
        }
        
        for (i = 0; i < num_threads; i++)
        {
                pthread_join(tid[i], NULL);
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
