#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <scic/fft.h>
#include "include/test.h"

int cml_count_tests = 0;
int cml_count_failedtests = 0;
int cml_count_failures = 0;

static double complex
test_result[32] = {
#include "data/results.dat"
};

int
run_fft_tests()
{
        CATEGORY_BEGIN(FFT)
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
                        
                        /* Do FFT */
                        result = scic_fft_pfa(input, 30, 2, 15);
                        
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
        clock_t cl = clock();

        run_fft_tests();

        (cml_count_failedtests > 0) ?
        printf(RED) :
        printf(GREEN);

        printf("\n%d/%d tests passed overall, %d failures\n" RESET "%Lg%s\n",
               cml_count_tests - cml_count_failedtests,
               cml_count_tests,
               cml_count_failures,
               (long double) (clock()-cl)/CLOCKS_PER_SEC,
               "s"
               );

        return (cml_count_failedtests > 0);
}