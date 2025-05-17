#include "log.h"
#include <stdarg.h>

static FILE *log_fp = NULL;

int log_open(const char *filename) {
    log_fp = fopen(filename, "a");
    return log_fp ? 0 : -1;
}

void log_write(const char *format, ...) {
    if (!log_fp) return;
    va_list args;
    va_start(args, format);
    vfprintf(log_fp, format, args);
    va_end(args);
    fflush(log_fp);
}

void log_close() {
    if (log_fp) {
        fclose(log_fp);
        log_fp = NULL;
    }
}
