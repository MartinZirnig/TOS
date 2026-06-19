#ifndef STRING_CONVERSIONS_H
#define STRING_CONVERSIONS_H

#include "types.h"


char* utos8(uint8 value);
char* itos8(int8 value);
char* utos16(uint16 value);
char* itos16(int16 value);
char* utos32(uint32 value);
char* itos32(int32 value);


uint8 stou8(char* ptr);
int8 stoi8(char* ptr);
uint16 stou16(char* ptr);
int16 stoi16(char* ptr);
uint32 stou32(char* ptr);
int32 stoi32(char* ptr);

#if defined(__x86_64__)

#define itos(a) _Generic((a), \
    int8: itos8, \
    int16: itos16, \
    int32: itos32, \
    int64: itos64, \
    uint8: utos8, \
    uint16: utos16, \
    uint32: utos32, \
    uint64: utos64  \
)(a)

char* utos64(uint64 value);
char* itos64(int64 value);

uint64 stou64(char* ptr);
int64 stoi64(char* ptr);
#else 

#define itos(a) _Generic((a), \
    int8: itos8, \
    int16: itos16, \
    int32: itos32, \
    uint8: utos8, \
    uint16: utos16, \
    uint32: utos32 \
)(a)

#endif

#endif