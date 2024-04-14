
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

void __LOG(const char *file, int line, const char *fmt, ...){
	va_list args;
#ifdef __LOG_TO_FILE
    static FILE *dst = NULL;
    if(dst == NULL) {
        dst = fopen(__LOG_TO_FILE, "wb");
        setbuf(dst, NULL);
    }
#else
	FILE *dst = stdout;
#endif
	va_start(args, fmt);
	fprintf(dst, "%s:%d: ", file, line < 0 ? -line : line);
	vfprintf(dst, fmt, args);
	fputc('\n', dst);
	va_end(args);
	if(line < 0) {
		exit(-1);
	}
}