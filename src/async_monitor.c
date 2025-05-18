// async_monitor.c
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/timerfd.h>
#include <sys/epoll.h>
#include <sys/statvfs.h>
#include <aio.h>
#include <errno.h>
#include <time.h>
#include <signal.h>

// --- 全域變數，用於 signal 處理（讓 Ctrl+C 能正常結束）---
volatile sig_atomic_t quit = 0;
void handle_sigint(int sig) { quit = 1; }

// --- 取得 CPU 使用率 ---
float get_cpu_usage()
{
    static long long last_user = 0, last_nice = 0, last_system = 0, last_idle = 0;
    FILE *fp = fopen("/proc/stat", "r");
    if (!fp) return -1;

    char buf[256];
    fgets(buf, sizeof(buf), fp);
    fclose(fp);

    long long user, nice, system, idle;
    sscanf(buf, "cpu %lld %lld %lld %lld", &user, &nice, &system, &idle);

    long long total_diff = (user + nice + system + idle) - (last_user + last_nice + last_system + last_idle);
    long long idle_diff  = idle - last_idle;

    float usage = 0.0;
    if (total_diff != 0)
        usage = (1.0 - (float)idle_diff / total_diff) * 100.0;

    last_user = user; last_nice = nice; last_system = system; last_idle = idle;
    return usage;
}

// --- 取得記憶體使用率 ---
float get_mem_usage()
{
    FILE *fp = fopen("/proc/meminfo", "r");
    if (!fp) return -1;
    char line[256];
    long total = 0, available = 0;
    while (fgets(line, sizeof(line), fp)) {
        if (sscanf(line, "MemTotal: %ld kB", &total) == 1) continue;
        if (sscanf(line, "MemAvailable: %ld kB", &available) == 1) break;
    }
    fclose(fp);
    if (total == 0) return -1;
    float usage = (1.0 - (float)available / total) * 100.0;
    return usage;
}

// --- 取得磁碟使用率 ---
float get_disk_usage(const char *path)
{
    struct statvfs vfs;
    if (statvfs(path, &vfs) != 0) return -1;
    unsigned long total = vfs.f_blocks * vfs.f_frsize;
    if (total == 0) return -1;  // 避免除以0
    unsigned long available = vfs.f_bavail * vfs.f_frsize;
    float usage = (1.0 - (float)available / total) * 100.0;
    return usage;
}

// --- 非同步寫入 log ---
void aio_log_write(const char *log_path, const char *msg)
{
    struct aiocb cb; // 放區域變數，避免多執行緒共用
    int fd = open(log_path, O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (fd < 0) { perror("open log"); return; }

    memset(&cb, 0, sizeof(cb));
    cb.aio_fildes = fd;
    cb.aio_buf = (void *)msg;
    cb.aio_nbytes = strlen(msg);
    cb.aio_offset = 0; // O_APPEND，不用 offset

    if (aio_write(&cb) == -1) {
        perror("aio_write");
        close(fd);
        return;
    }
    // 等待寫入完成
    while (aio_error(&cb) == EINPROGRESS) ;
    if (aio_error(&cb) != 0)
        fprintf(stderr, "AIO write error\n");
    close(fd);
}

// --- 產生 timestamp ---
void gen_timestamp(char *buf, size_t buflen)
{
    time_t now = time(NULL);
    struct tm t;
    localtime_r(&now, &t);
    strftime(buf, buflen, "%Y-%m-%d %H:%M:%S", &t);
}

int main()
{
    // 註冊 Ctrl+C 處理（讓 Ctrl+C 能正常結束 while）
    signal(SIGINT, handle_sigint);

    // --- 1. 建立 timerfd 作為 1 秒定時器 ---
    int tfd = timerfd_create(CLOCK_MONOTONIC, 0);
    if (tfd < 0) { perror("timerfd_create"); return 1; }
    struct itimerspec ts = { .it_interval = {1, 0}, .it_value = {1, 0} };
    timerfd_settime(tfd, 0, &ts, NULL);

    // --- 2. 建立 epoll，監聽 timerfd ---
    int epfd = epoll_create1(0);
    struct epoll_event ev = { .events = EPOLLIN, .data.fd = tfd };
    epoll_ctl(epfd, EPOLL_CTL_ADD, tfd, &ev);

    printf("啟動非同步系統資源監控，按 Ctrl+C 結束\n");

    while (!quit) {
        struct epoll_event events[1];
        int n = epoll_wait(epfd, events, 1, -1);
        if (n < 0) break;
        if (n > 0 && events[0].data.fd == tfd) {
            uint64_t exp;
            read(tfd, &exp, sizeof(exp)); // 清除 timerfd

            // --- 收集系統資訊 ---
            float cpu = get_cpu_usage();
            float mem = get_mem_usage();
            float disk = get_disk_usage("/");

            char tsbuf[32];
            gen_timestamp(tsbuf, sizeof(tsbuf));
            char logbuf[256];
            snprintf(logbuf, sizeof(logbuf),
                "[%s] CPU:%.2f%% MEM:%.2f%% DISK:%.2f%%\n", tsbuf, cpu, mem, disk);

            printf("%s", logbuf);

            // --- 非同步寫入 log ---
            aio_log_write("async_monitor.log", logbuf);
        }
    }

    printf("\n結束監控。\n");
    close(tfd);
    close(epfd);
    return 0;
}
