#ifndef STD_H
#define STD_H

typedef unsigned char           byte;
typedef unsigned short          word;
typedef unsigned int            dword;
typedef unsigned long long      qword;

typedef signed char             sbyte;
typedef signed short            sword;
typedef signed int              sdword;
typedef signed long long        sqword;


typedef byte                    uint8;
typedef word                    uint16;
typedef dword                   uint32;
typedef qword                   uint64;

typedef sbyte                   int8;
typedef sword                   int16;
typedef sdword                  int32;
typedef sqword                  int64;


typedef __UINTPTR_TYPE__        uintptr;
typedef uintptr                 nuint;

typedef __INTPTR_TYPE__         intptr;
typedef intptr                  nint;


typedef unsigned char           bool;
typedef _Bool                   bit;
typedef volatile bit            state;

#define UINT8_MAX               0xFF
#define UINT16_MAX              0xFFFF
#define UINT32_MAX              0xFFFFFF
#define UINT64_MAX              0xFFFFFFFF

#define UINT8_MIN               0x00
#define UINT16_MIN              0x0000
#define UINT32_MIN              0x000000
#define UINT64_MIN              0x00000000

#define INT8_MAX                0x7F
#define INT16_MAX               0x7FFF
#define INT32_MAX               0x7FFFFF
#define INT64_MAX               0x7FFFFFFF

#define INT8_MIN                0x80
#define INT16_MIN               0x8000
#define INT32_MIN               0x800000
#define INT64_MIN               0x80000000

#define true                    1
#define false                   0
#define null                    (void*)0
#define none                    (nuint)0

#define or                      ||
#define and                     &&

#define safeptr(X) volatile X* volatile

#endif