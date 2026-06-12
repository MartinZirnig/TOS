#ifndef HW_IO_H
#define HW_IO_H
#include "std.h"

#ifdef __cplusplus
extern "C" {
#endif

void __kill__(byte status);

byte __in8__(word port);
word __in16__(word port);

void __out8__(word port, byte value);
void __out16__(word port, word value);

#if defined(__x86_64__)

dword __in32__(word port);
void __out32__(word port, dword value);

#endif

#ifdef __cplusplus
}
#endif

#endif