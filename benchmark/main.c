#include <stdlib.h>
 #include <stdio.h>
 #include <string.h>
 #include <time.h>
 #include <omp.h>

 #include <sparse.h>
 #include <scic/fft.h>

extern double dwalltime(void);

int
main(int argc, char **argv)
{
	size_t i, N, N1, N2, NUM_THREADS;
	double timetick;
	char *N2_flag;

	double complex *input;
	double complex *result;

	N = atoi(sparse_flag("N", "10000", argc, argv));
	N1 = atoi(sparse_flag("N1", "5", argc, argv));
	N2_flag = sparse_flag("N2", "undefined", argc, argv);
	N2 = strcmp(N2_flag, "undefined") == 0 ? N / N1 : (size_t)atoi(N2_flag);
	NUM_THREADS = atoi(sparse_flag("threads", "2", argc, argv));

	input = (double complex *) malloc(sizeof(double complex) * N);

	printf("N: %ld - N1: %ld - N2: %ld - Threads: %ld\n\n", N, N1, N2, NUM_THREADS);

	/* Init input */
	for (i = 0; i < N; i++)
	{
		input[i] = ((double)i) + 0.0 * I;
	}

	printf("Benchmarks for N = %ld\n\n", N);

	timetick = dwalltime();

	result = scic_fft(input, N, N1, N2);

	printf("  %-10s %fs\n", "Serial", dwalltime() - timetick);

	timetick = dwalltime();

	result = scic_pthread_fft(input, N, N1, N2, NUM_THREADS);

	printf("  %-10s %fs\n", "Pthread", dwalltime() - timetick);

	timetick = dwalltime();

	result = scic_openmp_fft(input, N, N1, N2, NUM_THREADS);

	printf("  %-10s %fs\n", "OpenMp", dwalltime() - timetick);

	(void)result;

	return 0;
}
