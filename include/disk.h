#ifndef DISK_H
#define DISK_H

typedef struct {
    unsigned long total;   // MB
    unsigned long used;    // MB
    unsigned long avail;   // MB
} DiskInfo;

int get_disk_info(DiskInfo *disk, const char *path);

#endif
