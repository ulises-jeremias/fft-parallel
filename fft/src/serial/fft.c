#include <stdlib.h>
#include <math.h>
#include <scic/fft.h>

extern double complex complex_polar(double r, double theta);

/**
 * Implements the Good-Thomas FFT algorithm.
 *
 * @expects N1 and N2 must be relatively prime
 * @expects N1*N2 = N
 * 
 * @param double complex *, input
 * @param size_t N
 * @param size_t N1
 * @param size_t N2
 * 
 * @return double complex *
 */
double complex *
scic_fft_pfa(double complex *input, size_t N, size_t N1, size_t N2)
{
        double complex **columns, **rows, *output;
        size_t k1, k2, z;

        /* Allocate columnwise matrix */
        columns = (double complex **) malloc(sizeof(double complex *) * N1);

        for (k1 = 0; k1 < N1; k1++)
        {
                columns[k1] = (double complex *) malloc(sizeof(double complex) * N2);
        }
        
        /* Allocate rowwise matrix */
        rows = (double complex **) malloc(sizeof(double complex *) * N2);

        for (k2 = 0; k2 < N2; k2++)
        {
                rows[k2] = (double complex *) malloc(sizeof(double complex) * N1);
        }
        
        /* Reshape input into N1 columns (Using Good-Thomas Indexing) */
        for (z = 0; z < N; z++)
        {
                k1 = z % N1;
                k2 = z % N2;
                columns[k1][k2] = input[z];
        }
        
        /* Compute N1 DFTs of length N2 using naive method */
        for (k1 = 0; k1 < N1; k1++)
        {
                columns[k1] = scic_dft_naive(columns[k1], N2);
        }
        
        /* Transpose */
        for (k1 = 0; k1 < N1; k1++)
        {
                for (k2 = 0; k2 < N2; k2++)
                {
                        rows[k2][k1] = columns[k1][k2];
                }
        }
        
        /* Compute N2 DFTs of length N1 using naive method */
        for (k2 = 0; k2 < N2; k2++)
        {
                rows[k2] = scic_dft_naive(rows[k2], N1);
        }
        
        /* Flatten into single output (Using chinese remainder theorem) */
        output = (double complex *) malloc(sizeof(double complex) * N);
        
        for (k1 = 0; k1 < N1; k1++)
        {
                for (k2 = 0; k2 < N2; k2++)
                {
                        z = N1*k2 + N2*k1;
                        output[z%N] = rows[k2][k1];
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
 * 
 * @return double complex *
 * 
 */
double complex *
scic_fft(double complex *input, size_t N, size_t N1, size_t N2)
{
        double complex **columns, **rows, *output;
        size_t k1, k2;

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
        
        /* Reshape input into N1 columns */
        for (k1 = 0; k1 < N1; k1++)
        {
                for (k2 = 0; k2 < N2; k2++)
                {
                        columns[k1][k2] = input[N1*k2 + k1];
                }
        }

        /* Compute N1 DFTs of length N2 using naive method */
        for (k1 = 0; k1 < N1; k1++)
        {
                columns[k1] = scic_dft_naive(columns[k1], N2);
        }
        
        /* Multiply by the twiddle factors exp(-2*pi*k1*k2*i/N) and transpose */
        for (k1 = 0; k1 < N1; k1++)
        {
                for (k2 = 0; k2 < N2; k2++)
                {
                        rows[k2][k1] = complex_polar(1, -2*M_PI*k1*k2/N) * columns[k1][k2];
                }
        }
        
        /* Compute N2 DFTs of length N1 using naive method */
        for (k2 = 0; k2 < N2; k2++)
        {
                rows[k2] = scic_dft_naive(rows[k2], N1);
        }
        
        /* Flatten into single output */
        output = (double complex*) malloc(sizeof(double complex) * N);

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

        return output;
}
