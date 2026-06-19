#include "lock.h"
#include "std.h"

void __lock__(safeptr(Lock) lock) {
    __lock_unsafe__(&(lock->isLocked));
}

void __lock_unsafe__(safeptr(state) lock) {
    while (__atomic_test_and_set(lock, __ATOMIC_ACQUIRE)) {
        __asm__ volatile("pause");
    }
}

void __unlock__(safeptr(Lock) lock) {
    __unlock_unsafe__(&(lock->isLocked));
}

void __unlock_unsafe__(safeptr(state) lock) {
    __atomic_clear(lock, __ATOMIC_RELEASE);
}