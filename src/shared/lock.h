#ifndef LOCK_H
#define LOCK_H

#include "std.h"

typedef struct Lock
{
    state isLocked;
} Lock;

void __lock__(safeptr(Lock) lock);
void __lock_unsafe__(safeptr(state) lock);

void __unlock__(safeptr(Lock) lock);
void __unlock_unsafe__(safeptr(state) lock);

#endif