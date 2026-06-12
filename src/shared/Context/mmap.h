#ifndef MMAP_H
#define MMAP_H

#include "std.h"

typedef enum {
    MMAP_Available = 1,
    MMAP_Reserved = 2,
    MMAP_ACPI_Tmp = 3,
    MMAP_ACPI_NVS = 4,
    MMAP_Unusable = 5,
} MmapType;

typedef struct __attribute__((packed)) {
    qword address;
    qword length;
    dword type;
    dword flags;
} Mmap;

typedef struct __attribute__((packed)) {
    word size;
    Mmap maps[];
} Mmaps;

#endif