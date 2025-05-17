#include <sys/statvfs.h>
#include "disk.h"

int get_disk_info(DiskInfo *disk, const char *path) {
    struct statvfs fs;
    if (statvfs(path, &fs) != 0) return -1;
    disk->total  = fs.f_blocks * fs.f_frsize / 1024 / 1024;
    disk->avail  = fs.f_bavail * fs.f_frsize / 1024 / 1024;
    disk->used   = disk->total - disk->avail;
    return 0;
}
