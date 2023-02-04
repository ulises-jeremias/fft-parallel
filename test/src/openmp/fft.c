#include <stdlib.h>
 #include <stdio.h>
 #include <time.h>
 #include <scic/fft.h>
 #include "../../include/test.h"

extern double dwalltime(void);

int cml_count_tests;
int cml_count_failedtests;
int cml_count_failures;

static double complex
test_result[32] = {
 #include "../../data/results.dat"
};

int
run_fft_tests()
{
	CATEGORY_BEGIN(OpenMP FFT)
	{
		TEST_BEGIN(Cooley - Tukey)
		{
			double complex *input = (double complex *) malloc(sizeof(double complex) * 30);
			double complex *result;
			size_t i;

			/* Init input */
			for (i = 0; i < 30; i++)
			{
				input[i] = ((double)i) + 0.0 * I;
			}

			/* Do FFT */
			result = scic_openmp_fft(input, 30, 6, 5, 2);

			printf("\n\n");

			/* Compare results */
			for (i = 0; i < 30; i++)
			{
				EXPECT_FLOAT_EQ(creal(result[i]), creal(test_result[i]));
				EXPECT_FLOAT_EQ(cimag(result[i]), cimag(test_result[i]));
			}
		}
		TEST_END()
	}
	CATEGORY_END()

	return 0;
}

int
main()
{
	double timetick = dwalltime();

	run_fft_tests();

	(cml_count_failedtests > 0) ?
		printf(RED) :
		printf(GREEN);

	printf("\n%d/%d tests passed overall, %d failures\n" RESET "%fs\n",
	       cml_count_tests - cml_count_failedtests,
	       cml_count_tests,
	       cml_count_failures,
	       dwalltime() - timetick
	       );

	return (cml_count_failedtests > 0);
}
