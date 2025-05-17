#include <stdio.h>
#include "cpu.h"
#include "memory.h"
#include "disk.h"

int main() {
    // CPU
    float cpu_usage = get_cpu_usage();
    printf("CPU Usage: %.2f%%\n", cpu_usage);

    // Memory
    MemoryInfo mem;
    if (get_memory_info(&mem) == 0) {
        unsigned long mem_used = mem.total - mem.available;
        float mem_usage = 100.0 * mem_used / mem.total;
        printf("Memory Usage: %.2f%% (%lu MiB / %lu MiB)\n",
               mem_usage, mem_used/1024, mem.total/1024);
    } else {
        printf("Memory Usage: N/A\n");
    }

    // Disk
    DiskInfo disk;
    if (get_disk_info(&disk, "/") == 0) {
        float disk_usage = 100.0 * disk.used / disk.total;
        printf("Disk Usage: %.2f%% (%lu MiB / %lu MiB)\n",
               disk_usage, disk.used, disk.total);
    } else {
        printf("Disk Usage: N/A\n");
    }

    return 0;
}
