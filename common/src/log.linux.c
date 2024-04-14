
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

void __LOG(const char *file, int line, const char *fmt, ...){
	va_list args;
	va_start(args, fmt);
	fprintf(stdout, "%s:%d: ", file, line < 0 ? -line : line);
	vfprintf(stdout, fmt, args);
	fputc('\n', stdout);
	va_end(args);
	if(line < 0) {
		exit(-1);
	}
}