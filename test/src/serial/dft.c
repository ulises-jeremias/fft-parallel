#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <scic/fft.h>
#include "../../include/test.h"

extern double dwalltime();

int cml_count_tests = 0;
int cml_count_failedtests = 0;
int cml_count_failures = 0;

static double complex
test_result[32] = {
#include "../../data/results.dat"
};

int
run_dtf_tests()
{
        CATEGORY_BEGIN(DFT)
        {
                TEST_BEGIN(Naive definition)
                {
                        double complex *input = (double complex *) malloc(sizeof(double complex) * 30);
                        double complex *result;
                        int i;
                        
                        /* Init input */
                        for (i = 0; i < 30; i++)
                        {
                                input[i] = ((double) i) + 0.0 * I;
                        }
                        
                        /* Do DFT */
                        result = scic_dft_naive(input, 30);
                        
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

        run_dtf_tests();

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