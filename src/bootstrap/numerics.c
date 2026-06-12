#include "bootstrap.h"

uint64 div64(uint64 value, uint64 divider) {
    if (divider == 0) return 0;
    if (value < divider) return 0;

    uint64 quotient = 0;
    uint64 remnant = 0;

    for (int i = 63; i >= 0; i--) {
        remnant <<= 1;
        remnant |= (value >> i) & 1;

        if (remnant >= divider) {
            remnant -= divider;
            quotient |= (1ULL << i);
        }
    }

    return quotient;
}

uint64 mod64(uint64 value, uint64 modus) {
    if (modus == 0) return 0;
    if (value < modus) return value;

    uint64 remnant = 0;

    for (int i = 63; i >= 0; i--) {
        remnant <<= 1;
        remnant |= (value >> i) & 1;

        if (remnant >= modus) {
            remnant -= modus;
        }
    }

    return remnant;
}