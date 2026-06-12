#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include "std.h"
#include "lock.h"

typedef struct Allocator
{
    void* start;
    void* end;
    uint32 pageSize;
    Lock lock;
} Allocator;


safeptr(Allocator) __initialize_allocator__(void* start, void* end, void* memory, uint32 pageSize);
safeptr(Allocator) __create_allocator__(void* start, void* end, uint32 pageSize);

void* __malloc__(uint32 size, volatile safeptr(Allocator) allocator);
void* __calloc__(uint32 size, uint32 count, safeptr(Allocator) allocator);
bool __reserve__(void* address, uint32 size, safeptr(Allocator) allocator);
bool __free__(void* address, safeptr(Allocator) allocator);
void __freeAll__(safeptr(Allocator) allocator);

void __setSharedAllocator__(safeptr(Allocator) allocator);
void* malloc(uint32 size);
void* calloc(uint32 size, uint32 count);
bool reserve(void* address, uint32 size);
bool free(void* address);
void freeAll();

#endif