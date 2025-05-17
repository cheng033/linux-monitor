#include <stdio.h>
#include <string.h>
#include "memory.h"

int get_memory_info(MemoryInfo *mem) {
    FILE *fp = fopen("/proc/meminfo", "r");
    if (!fp) return -1;
    char line[256];
    unsigned long total = 0, available = 0;
    while (fgets(line, sizeof(line), fp)) {
        if (sscanf(line, "MemTotal: %lu kB", &total) == 1) {}
        if (sscanf(line, "MemAvailable: %lu kB", &available) == 1) {}
        if (total && available) break;
    }
    fclose(fp);
    mem->total = total;
    mem->available = available;
    return (total && available) ? 0 : -1;
}
