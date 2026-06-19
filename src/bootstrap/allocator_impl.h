#ifndef ALLOCATOR_IMPL_H
#define ALLOCATOR_IMPL_H

#include "bootstrap.h"

typedef struct
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

bool reserve(void* address, uint32 size);


#endif