#ifndef HW_IO_H
#define HW_IO_H
#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

void __kill__(byte status);

byte __in8__(word port);
word __in16__(word port);

void __ins8__(word port, void* destination);
void __ins16__(word port, void* destination);

void __rep_ins8__(word port, void* destination, uint32 times);
void __rep_ins16__(word port, void* destination, uint32 times);

void __out8__(word port, byte value);
void __out16__(word port, word value);

void __outs8__(word port, void* src);
void __outs16__(word port, void* src);

void __rep_outs8__(word port, void* src, uint32 times);
void __rep_outs16__(word port, void* src, uint32 times);


#if defined(__x86_64__)

dword __in32__(word port);
void __ins32__(word port, void* dest);
void __rep_ins32__(word port, void* dest, uint32 times);

void __out32__(word port, dword value);
void __outs32__(word port, void* src);
void __rep_outs32__(word port, void* src, uint32 times);

#endif

#ifdef __cplusplus
}
#endif

#endif