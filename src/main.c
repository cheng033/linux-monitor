#include <stdio.h>
#include <unistd.h>
#include <ncurses.h>
#include "cpu.h"
#include "memory.h"
#include "disk.h"
#include "log.h"

int main() {
    log_open("log/monitor.log");
    initscr();
    cbreak();
    noecho();
    curs_set(0);

    while (1) {
        clear();

        // CPU
        float cpu_usage = get_cpu_usage();

        // Memory
        MemoryInfo mem;
        float mem_usage = 0;
        if (get_memory_info(&mem) == 0) {
            unsigned long mem_used = mem.total - mem.available;
            mem_usage = 100.0 * mem_used / mem.total;
        }

        // Disk
        DiskInfo disk;
        float disk_usage = 0;
        if (get_disk_info(&disk, "/") == 0) {
            disk_usage = 100.0 * disk.used / disk.total;
        }

        mvprintw(1, 2, "CPU Usage   : %.2f%%", cpu_usage);
        mvprintw(2, 2, "Memory Usage: %.2f%%", mem_usage);
        mvprintw(3, 2, "Disk Usage  : %.2f%%", disk_usage);
        mvprintw(5, 2, "Press 'q' to quit.");
        refresh();

        log_write("CPU:%.2f%%, Memory:%.2f%%, Disk:%.2f%%\n",
                  cpu_usage, mem_usage, disk_usage);

        timeout(1000); // 1 秒刷新
        int ch = getch();
        if (ch == 'q' || ch == 'Q')
            break;
    }

    endwin();
    log_close();
    return 0;
}
