#include "memory.h"

void copy(const void* src, void* dest, uint32 size) {
    const byte* source = (const byte*)src;
    byte* destination = (byte*)dest;

    for (uint32 i = 0; i < size; i++) {
        *destination++ = *source++;
    }
}

void fill(void* dest, uint32 size, byte template) {
    byte* destination = (byte*)dest;

    for (uint32 i = 0; i < size; i++) {
        destination[0] = template;
        destination += 1;
    }
}
void templateFill(void* dest, uint32 destSize, const void* pattern, uint32 patternSize) {
    byte* destination = (byte*)dest;
    const byte* source = (const byte*)pattern;

    uint32 written = 0;

    while (written < destSize) {
        for (uint32 i = 0; i < patternSize && written < destSize; i++) {
            *destination++ = source[i];
            written++;
        }
    }
}
