#include "bootstrap.h"
#include "HW_IO.h"
#include "VGA.h"


volatile void sleep(uint32 millis) {
    volatile uint32 limit = millis * 10000;

    for (volatile uint32 i = 0; i < limit; i++) {
        __asm__ volatile ("pause");
    }

    return;
}