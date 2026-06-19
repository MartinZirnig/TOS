#ifndef BTS_OUT_H
#define BTS_OUT_H

#include "std.h"
#include "addr.h"

#define MAX_SUPPORTED_MEMORY_SEGMENTS               0xF

typedef struct
{
    uint64 bot;
    uint64 top;
} Range_64;

typedef struct
{
    qword PML4;
    qword maps;
    qword kernelPhysical;

    uint32 rangeCount;
    Range_64 availableRanges[MAX_SUPPORTED_MEMORY_SEGMENTS];
} PagingInit_64Bit;

uint64 div64(uint64 value, uint64 divider);
uint64 mod64(uint64 value, uint64 modus);

str uint64ToString(uint64 value);
void print(str message);
void printInfo(str message);
void printSuccess(str message);
void printError(str message);

void sleep(uint32 millis);

#endif