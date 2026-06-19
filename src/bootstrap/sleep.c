#include "bootstrap.h"


volatile void sleep(uint32 millis) {
    volatile uint32 limit = millis * 1000;

    for (volatile uint32 i = 0; i < limit; i++) {
        __asm__ volatile ("pause");
    }

    return;
}