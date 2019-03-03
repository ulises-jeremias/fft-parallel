#include <stdlib.h>
#include <math.h>
#include <complex.h>

double complex
complex_polar(double r, double theta)
{                               /* return z = r exp(i theta) */
        return r * cos(theta) + r * sin(theta) * I;
}
