#ifndef MEMORY_H
#define MEMORY_H

typedef struct {
    unsigned long total; // KB
    unsigned long available; // KB
} MemoryInfo;

int get_memory_info(MemoryInfo *mem);

#endif
