#include <assert.h>
#include <stdio.h>
#include "memory.h"

int main() {
    MemoryInfo mem;
    int ret = get_memory_info(&mem);
    assert(ret == 0);

    printf("Memory total    : %lu KB\n", mem.total);
    printf("Memory available: %lu KB\n", mem.available);

    float usage = (mem.total - mem.available) * 100.0 / mem.total;
    printf("Memory usage = %.2f%%\n", usage);
    assert(usage >= 0.0 && usage <= 100.0);
    printf("[PASS] Memory usage in valid range.\n");
    return 0;
}

