// tests/test_cpu.c
#include <assert.h>
#include <stdio.h>
#include "../include/cpu.h"

int main() {
    float usage = get_cpu_usage();
    // 因為第一次取樣會是0或不準，建議呼叫兩次
    usage = get_cpu_usage();
    printf("CPU usage = %.2f%%\n", usage);

    // 測試值應該在合理範圍內
    assert(usage >= 0.0 && usage <= 100.0);

    printf("[PASS] CPU usage in valid range.\n");
    return 0;
}

