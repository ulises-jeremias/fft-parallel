#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <omp.h>
#include <math.h>

#include <sparse.h>
#include <scic/fft.h>

extern double dwalltime(void);

const int N1 = 5;
const int INPUTS_N[] = { 1024, 2048, 4096, 8192, 16384, 32768 };
const int NUM_THREADS[] = { 1, 2, 4, 8 };

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))

void
run_pthread_benchmark(size_t N, size_t N1, size_t N2, size_t NUM_THREADS, size_t TIMES)
{
	size_t i;
	double timetick, serial_sum, serial_avg, serial_std_dev, parallel_sum, parallel_avg, parallel_std_dev, speedup, efficiency, overhead;

	double complex *input;
	double complex *result;

	input = (double complex *) malloc(sizeof(double complex) * N);

	/* Init input */
	for (i = 0; i < N; i++)
	{
		input[i] = ((double)i) + 0.0 * I;
	}

	printf("Benchmarks for N = %ld:\n", N);

	// Run benchmark N times calculating the average and standard deviation
	// and then print speedup, efficiency and overhead

	for (i = 0; i < TIMES; i++)
	{
		timetick = dwalltime();

		result = scic_fft(input, N, N1, N2);

		serial_sum += dwalltime() - timetick;
	}

	serial_avg = serial_sum / TIMES;
	serial_std_dev = 0;

	for (i = 0; i < TIMES; i++)
	{
		timetick = dwalltime();

		result = scic_pthread_fft(input, N, N1, N2, NUM_THREADS);

		parallel_sum += dwalltime() - timetick;
	}

	parallel_avg = parallel_sum / TIMES;
	parallel_std_dev = 0;

	speedup = serial_avg / parallel_avg;
	efficiency = speedup / NUM_THREADS;
	overhead = (1 - efficiency) * 100;

	printf("Serial: %f+/-%f - Parallel: %f+/-%f - Speedup: %f - Efficiency: %f - Overhead: %f\n\n", serial_avg, serial_std_dev, parallel_avg, parallel_std_dev, speedup, efficiency, overhead);

	(void)result;

	free(input);
}

void
run_openmp_benchmark(size_t N, size_t N1, size_t N2, size_t NUM_THREADS, size_t TIMES)
{
	size_t i;
	double timetick, serial_sum, serial_avg, serial_std_dev, parallel_sum, parallel_avg, parallel_std_dev, speedup, efficiency, overhead;
	double complex *input;
	double complex *result;

	input = (double complex *) malloc(sizeof(double complex) * N);

	/* Init input */
	for (i = 0; i < N; i++)
	{
		input[i] = ((double)i) + 0.0 * I;
	}

	printf("Benchmarks for N = %ld:\n", N);

	// Run benchmark N times calculating the average and standard deviation
	// and then print speedup, efficiency and overhead

	for (i = 0; i < TIMES; i++)
	{
		timetick = dwalltime();

		result = scic_fft(input, N, N1, N2);

		serial_sum += dwalltime() - timetick;
	}

	serial_avg = serial_sum / TIMES;
	serial_std_dev = 0;

	for (i = 0; i < TIMES; i++)
	{
		timetick = dwalltime();

		result = scic_openmp_fft(input, N, N1, N2, NUM_THREADS);

		parallel_sum += dwalltime() - timetick;
	}

	parallel_avg = parallel_sum / TIMES;
	parallel_std_dev = 0;

	speedup = serial_avg / parallel_avg;
	efficiency = speedup / NUM_THREADS;
	overhead = (1 - efficiency) * 100;

	printf("Serial: %f+/-%f - Parallel: %f+/-%f - Speedup: %f - Efficiency: %f - Overhead: %f\n\n", serial_avg, serial_std_dev, parallel_avg, parallel_std_dev, speedup, efficiency, overhead);

	(void)result;

	free(input);
}

int
main(int argc, char **argv)
{
	size_t i, j, times;
	int N, N2, num_threads;

	times = atoi(sparse_flag("times", "1", argc, argv));

	for (i = 0; i < ARRAY_SIZE(NUM_THREADS); i++)
	{
		for (j = 0; j < ARRAY_SIZE(INPUTS_N); j++)
		{
			N = INPUTS_N[j];
			N2 = N / N1;
			num_threads = NUM_THREADS[i];

			run_pthread_benchmark(N, N1, N2, num_threads, times);
			run_openmp_benchmark(N, N1, N2, num_threads, times);
		}
	}

	return 0;
}
