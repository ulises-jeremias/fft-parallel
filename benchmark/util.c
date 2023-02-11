#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>

FILE *log_file;

double
dwalltime()
{
	double sec;
	struct timeval tv;

	gettimeofday(&tv, NULL);
	sec = tv.tv_sec + tv.tv_usec / 1000000.0;

	return sec;
}

void
print_log(const char *s, ...)
{
	va_list args;

        va_start(args, s);

        vfprintf(log_file, s, args);
        fflush(log_file);

        va_end(args);
}

void init_log(const char *filename, const char *mode)
{
	log_file = fopen(filename, mode);
	print_log("Log file path: %s\n", filename);
}
