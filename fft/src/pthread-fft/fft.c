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
        pthread_barrier_t *barrier;

        size_t tid;
        size_t num_threads;

        double complex **columns;
        double complex **rows;
        double complex *output;

        double complex *input;
        size_t N;
        size_t N1;
        size_t N2;
} scic_pthread_arg_t;

extern double complex complex_polar(double r, double theta);

void *
__scic_pthread_init(void *arg)
{
        scic_pthread_arg_t *p_arg = (scic_pthread_arg_t *) arg;

        double complex **columns, **rows;
        size_t k1, k2, tid, num_threads, N1, N2, limit_N1, limit_N2;

        columns      = p_arg->columns,
        rows         = p_arg->rows;
        tid          = p_arg->tid,
        num_threads  = p_arg->num_threads,
        N1           = p_arg->N1,
        N2           = p_arg->N2;

        limit_N1 = (1 + tid) * N1/num_threads,
        limit_N2 = (1 + tid) * N2/num_threads;

        for (k1 = tid*N1/num_threads; k1 < limit_N1; k1++)
        {
                columns[k1] = (double complex*) malloc(sizeof(double complex) * N2);
        }
        
        for (k2 = tid*N2/num_threads; k2 < limit_N2; k2++)
        {
                rows[k2] = (double complex*) malloc(sizeof(double complex) * N1);
        }

        return NULL;
}

void *
__scic_pthread_compute_fft(void *arg)
{
        scic_pthread_arg_t *p_arg = (scic_pthread_arg_t *) arg;

        double complex **columns, **rows, *output, *input;
        size_t k1, k2, tid, num_threads, N, N1, N2, limit_N1, limit_N2;

        columns      = p_arg->columns,
        rows         = p_arg->rows;
        output       = p_arg->output;
        input        = p_arg->input;
        tid          = p_arg->tid,
        num_threads  = p_arg->num_threads,
        N            = p_arg->N;
        N1           = p_arg->N1,
        N2           = p_arg->N2;

        limit_N1 = (1 + tid) * N1/num_threads,
        limit_N2 = (1 + tid) * N2/num_threads;

        /* Reshape input into N1 columns */
        for (k1 = tid*N1/num_threads; k1 < limit_N1; k1++)
        {
                for (k2 = 0; k2 < N2; k2++)
                {
                        columns[k1][k2] = input[N1*k2 + k1];
                }
        }

        /* Compute N1 DFTs of length N2 using naive method */
        for (k1 = tid*N1/num_threads; k1 < limit_N1; k1++)
        {
                columns[k1] = scic_dft_naive(columns[k1], N2);
        }
        
        /* Multiply by the twiddle factors exp(-2*pi*k1*k2*i/N) and transpose */
        for (k1 = tid*N1/num_threads; k1 < limit_N1; k1++)
        {
                for (k2 = 0; k2 < N2; k2++)
                {
                        rows[k2][k1] = complex_polar(1, -2*M_PI*k1*k2/N) * columns[k1][k2];
                }
        }

        pthread_barrier_wait(p_arg->barrier);
        
        /* Compute N2 DFTs of length N1 using naive method */
        for (k2 = tid*N2/num_threads; k2 < limit_N2; k2++)
        {
                rows[k2] = scic_dft_naive(rows[k2], N1);
        }

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

        pthread_t threads[num_threads];
        pthread_barrier_t barrier;

        scic_pthread_arg_t **args;
        size_t i, k1, k2;

        long rc;

        pthread_barrier_init(&barrier, NULL, num_threads);

        args = malloc(sizeof(scic_pthread_arg_t *) * num_threads);

        /* Allocate columnwise matrix */
        columns = (double complex**) malloc(sizeof(double complex*) * N1);

        /* Allocate rowwise matrix */
        rows = (double complex**) malloc(sizeof(double complex*) * N2);

        /* Allocate output array */
        output = (double complex*) malloc(sizeof(double complex) * N);

        for (i = 0; i < num_threads; i++)
        {
                args[i] = malloc(sizeof(scic_pthread_arg_t));

                *args[i] = (scic_pthread_arg_t) {
                        .barrier = &barrier,
                        .tid = i,
                        .num_threads = num_threads,
                        
                        .columns = columns,
                        .rows = rows,
                        .output = output,

                        .input = input,
                        .N = N,
                        .N1 = N1,
                        .N2 = N2
                };
        }

        for (i = 0; i < num_threads; i++)
        {
                pthread_create(&threads[i], NULL, &__scic_pthread_init, (void *) args[i]);
        }
        
        for (i = 0; i < num_threads; i++)
        {
                rc = pthread_join(threads[i], NULL);

                if (rc)
                {
                        printf("ERROR; return code from pthread_join() is %ld\n", rc);
                        exit(-1);
                }
        }

        for (i = 0; i < num_threads; i++)
        {
                pthread_create(&threads[i], NULL, &__scic_pthread_compute_fft, (void *) args[i]);
        }
        
        for (i = 0; i < num_threads; i++)
        {
                rc = pthread_join(threads[i], NULL);

                if (rc)
                {
                        printf("ERROR; return code from pthread_join() is %ld\n", rc);
                        exit(-1);
                }
        }

        /* Flatten into single output */
        for (k1 = 0; k1 < N1; k1++)
        {
                for (k2 = 0; k2 < N2; k2++)
                {
                        output[N2*k1 + k2] = rows[k2][k1];
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

        pthread_barrier_destroy(&barrier);

        return output;
}
