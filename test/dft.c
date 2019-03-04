#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <time.h>
#include <scic/fft.h>
#include "include/test.h"

int cml_count_tests = 0;
int cml_count_failedtests = 0;
int cml_count_failures = 0;

static double complex
test_result[32] = {
        435.000000 + 0.000000 * I,
        -15.000000 + 142.715467 * I, 
        -15.000000 + 70.569452 * I, 
        -15.000000 + 46.165253 * I, 
        -15.000000 + 33.690552 * I, 
        -15.000000 + 25.980762 * I, 
        -15.000000 + 20.645729 * I, 
        -15.000000 + 16.659188 * I, 
        -15.000000 + 13.506061 * I, 
        -15.000000 + 10.898138 * I, 
        -15.000000 + 8.660254 * I, 
        -15.000000 + 6.678430 * I, 
        -15.000000 + 4.873795 * I, 
        -15.000000 + 3.188348 * I, 
        -15.000000 + 1.576564 * I, 
        -15.000000 + -0.000000 * I, 
        -15.000000 + -1.576564 * I, 
        -15.000000 + -3.188348 * I, 
        -15.000000 + -4.873795 * I, 
        -15.000000 + -6.678430 * I, 
        -15.000000 + -8.660254 * I, 
        -15.000000 + -10.898138 * I, 
        -15.000000 + -13.506061 * I, 
        -15.000000 + -16.659188 * I, 
        -15.000000 + -20.645729 * I, 
        -15.000000 + -25.980762 * I, 
        -15.000000 + -33.690552 * I, 
        -15.000000 + -46.165253 * I, 
        -15.000000 + -70.569452 * I, 
        -15.000000 + -142.715467 * I
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
        clock_t cl = clock();

        run_dtf_tests();

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