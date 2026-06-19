#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include "types.h"
#include "./../Lock/lock.h"

void* malloc(uint32 size);
void* calloc(uint32 size, uint32 count);
bool reserve(void* address, uint32 size);
bool free(void* address);
void freeAll();

#endif