#ifndef MEMORY_H
#define MEMORY_H

#include "std.h"
#include "allocator.h"
#include "range.h"

void copy(const void* src, void* dest, uint32 size);
void fill(void* dest, uint32 size, byte template);
void templateFill(void* dest, uint32 destSize, const void* template, uint32 templateSize);

#endif