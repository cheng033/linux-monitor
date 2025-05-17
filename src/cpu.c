#include <stdio.h>
#include <unistd.h>
#include "cpu.h"

float get_cpu_usage(void) {
    FILE *fp;
    char line[256];
    unsigned long user1, nice1, sys1, idle1, user2, nice2, sys2, idle2;

    fp = fopen("/proc/stat", "r");
    if (!fp) return -1.0;
    fgets(line, sizeof(line), fp);
    sscanf(line, "cpu  %lu %lu %lu %lu", &user1, &nice1, &sys1, &idle1);
    fclose(fp);
    sleep(1);
    fp = fopen("/proc/stat", "r");
    if (!fp) return -1.0;
    fgets(line, sizeof(line), fp);
    sscanf(line, "cpu  %lu %lu %lu %lu", &user2, &nice2, &sys2, &idle2);
    fclose(fp);

    unsigned long total1 = user1 + nice1 + sys1 + idle1;
    unsigned long total2 = user2 + nice2 + sys2 + idle2;
    unsigned long total_diff = total2 - total1;
    unsigned long idle_diff = idle2 - idle1;

    if (total_diff == 0) return 0.0;
    return 100.0 * (total_diff - idle_diff) / total_diff;
}
