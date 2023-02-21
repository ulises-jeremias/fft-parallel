#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <omp.h>
#include <math.h>

#include <benchmark.h>
#include <sparse.h>
#include <scic/fft.h>

const size_t N1 = 5;
const int INPUTS_N[] = { 1024, 2048, 4096, 8192, 16384, 32768 };
const int NUM_THREADS[] = { 1, 2, 4, 8 };

size_t input_size_idx;
double serial_avgs[] = { 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000 };
double serial_std_devs[] = { 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000 };

// Assert that the input size is the same than serial_avgs and serial_std_devs
static_assert(ARRAY_SIZE(INPUTS_N) == ARRAY_SIZE(serial_avgs), "serial_avgs and INPUTS_N must have the same size");
static_assert(ARRAY_SIZE(INPUTS_N) == ARRAY_SIZE(serial_std_devs), "serial_std_devs and INPUTS_N must have the same size");

void
run_serial(size_t N, size_t N1, size_t N2, size_t TIMES)
{
	size_t i;
	double timetick, serial_sum = 0.0, serial_avg;

	double complex *input;
	double complex *result;

	input = (double complex *) malloc(sizeof(double complex) * N);

	/* Init input */
	for (i = 0; i < N; i++)
	{
		input[i] = ((double)i) + 0.0 * I;
	}

	// Run benchmark N times calculating the average and standard deviation
	// and then print speedup, efficiency and overhead

	for (i = 0; i < TIMES; i++)
	{
		timetick = dwalltime();

		result = scic_fft(input, N, N1, N2);

		serial_sum += dwalltime() - timetick;
	}

	serial_avg = serial_sum / TIMES;
	serial_avgs[input_size_idx] = serial_avg;
	serial_std_devs[input_size_idx] = sqrt((serial_sum - serial_avg * serial_avg) / TIMES);

        // print a LaTeX table row containing the values \emph{N} & \emph{Tiempo}
        // for the Tiempo columns use 3 decimal places
        // for the N column use 0 decimal places
        // use the & symbol to separate the columns
        // use the \\ symbol to separate the rows
        print_log("%ld & %.3f $\\pm$ %.3f \\\\\n", N, serial_avg, serial_std_devs[input_size_idx]);

        (void)result;

	free(input);
}

void
run_pthread_benchmark(size_t N, size_t N1, size_t N2, size_t NUM_THREADS, size_t TIMES)
{
	size_t i;
	double timetick, parallel_sum = 0.0, parallel_avg, parallel_std_dev, speedup, efficiency, total_overhead;

	double complex *input;
	double complex *result;

	input = (double complex *) malloc(sizeof(double complex) * N);

	/* Init input */
	for (i = 0; i < N; i++)
	{
		input[i] = ((double)i) + 0.0 * I;
	}

	// Run benchmark N times calculating the average and standard deviation
	// and then print speedup, efficiency and overhead

	for (i = 0; i < TIMES; i++)
	{
		timetick = dwalltime();

		result = scic_pthread_fft(input, N, N1, N2, NUM_THREADS);

		parallel_sum += dwalltime() - timetick;
	}

	parallel_avg = parallel_sum / TIMES;
	parallel_std_dev = sqrt((parallel_sum - parallel_avg * parallel_avg) / TIMES);

	speedup = serial_avgs[input_size_idx] / parallel_avg;
	efficiency = speedup / NUM_THREADS;
	total_overhead = NUM_THREADS * parallel_avg - serial_avgs[input_size_idx];

        // print a LaTeX table row containing the values \emph{N} & \emph{Tiempo} & \emph{Speedup} & \emph{Eficiencia} & \emph{Overhead}
        // for the Tiempo, Speedup, Eficiencia and Overhead columns use 3 decimal places
        // for the N column use 0 decimal places
        // use the & symbol to separate the columns
        // use the \\ symbol to separate the rows
        print_log("%ld & %.3f $\\pm$ %.3f & %.3f & %.3f & %.3f \\\\\n", N, parallel_avg, parallel_std_dev, speedup, efficiency, total_overhead);

        (void)result;

	free(input);
}

void
run_openmp_benchmark(size_t N, size_t N1, size_t N2, size_t NUM_THREADS, size_t TIMES)
{
	size_t i;
	double timetick, parallel_sum = 0.0, parallel_avg, parallel_std_dev, speedup, efficiency, total_overhead;
	double complex *input;
	double complex *result;

	input = (double complex *) malloc(sizeof(double complex) * N);

	/* Init input */
	for (i = 0; i < N; i++)
	{
		input[i] = ((double)i) + 0.0 * I;
	}

	// Run benchmark N times calculating the average and standard deviation
	// and then print speedup, efficiency and overhead

	for (i = 0; i < TIMES; i++)
	{
		timetick = dwalltime();

		result = scic_openmp_fft(input, N, N1, N2, NUM_THREADS);

		parallel_sum += dwalltime() - timetick;
	}

	parallel_avg = parallel_sum / TIMES;
	parallel_std_dev = sqrt((parallel_sum - parallel_avg * parallel_avg) / TIMES);

	speedup = serial_avgs[input_size_idx] / parallel_avg;
	efficiency = speedup / NUM_THREADS;
        total_overhead = NUM_THREADS * parallel_avg - serial_avgs[input_size_idx];

        // print a LaTeX table row containing the values \emph{N} & \emph{Tiempo} & \emph{Speedup} & \emph{Eficiencia} & \emph{Overhead}
        // for the Tiempo, Speedup, Eficiencia and Overhead columns use 3 decimal places
        // for the N column use 0 decimal places
        // use the & symbol to separate the columns
        // use the \\ symbol to separate the rows
        print_log("%ld & %.3f $\\pm$ %.3f & %.3f & %.3f & %.3f \\\\\n", N, parallel_avg, parallel_std_dev, speedup, efficiency, total_overhead);

        (void)result;

	free(input);
}

int
main(int argc, char **argv)
{
	size_t i, times, N, N2, num_threads;
	char *log_file_path;

	times = atoi(sparse_flag("times", "1", argc, argv));
	log_file_path = sparse_flag("log", "log.txt", argc, argv);

	init_log(log_file_path, "w");

	print_log("Initializing serial benchmarks...\n");

	for (input_size_idx = 0; input_size_idx < ARRAY_SIZE(INPUTS_N); input_size_idx++)
	{
		N = INPUTS_N[input_size_idx];
		N2 = N / N1;
		run_serial(N, N1, N2, times);
	}

	print_log("Done initializing serial benchmarks...\n\n");

	for (i = 0; i < ARRAY_SIZE(NUM_THREADS); i++)
	{
                num_threads = NUM_THREADS[i];

                // print a LaTex row such us \rowcolor{black!45} \multicolumn{5}{l}{\color{white}N hilos}
                print_log("\\rowcolor{black!45} \\multicolumn{5}{l}{\\color{white}%ld hilos} \\\\\n", num_threads);

		for (input_size_idx = 0; input_size_idx < ARRAY_SIZE(INPUTS_N); input_size_idx++)
		{
			N = INPUTS_N[input_size_idx];
			N2 = N / N1;

			run_pthread_benchmark(N, N1, N2, num_threads, times);
		}

                print_log("\n");
	}

        for (i = 0; i < ARRAY_SIZE(NUM_THREADS); i++)
        {
                num_threads = NUM_THREADS[i];

                // print a LaTex row such us \rowcolor{black!45} \multicolumn{5}{l}{\color{white}N hilos}
                print_log("\\rowcolor{black!45} \\multicolumn{5}{l}{\\color{white}%ld hilos} \\\\\n", num_threads);

                for (input_size_idx = 0; input_size_idx < ARRAY_SIZE(INPUTS_N); input_size_idx++)
                {
                        N = INPUTS_N[input_size_idx];
                        N2 = N / N1;

                        run_openmp_benchmark(N, N1, N2, num_threads, times);

                }

                print_log("\n");
        }

        return 0;
}
