#ifndef LOG_H
#define LOG_H

#include <stdio.h>

int log_open(const char *filename);
void log_write(const char *format, ...);
void log_close();

#endif
