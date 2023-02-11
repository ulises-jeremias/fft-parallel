#ifndef BENCHMARK_H_
#define BENCHMARK_H_

#include <stdio.h>

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))

void init_log(const char *filename, const char *mode);
void print_log(const char *s, ...);
double dwalltime(void);

#endif
