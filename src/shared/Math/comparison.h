#ifndef MATH_COMPARISON_H
#define MATH_COMPARISON_H

#include "std.h"

#define max(a, b) _Generic((a), \
    int8: max8, \
    int16: max16, \
    int32: max32, \
    int64: max64, \
    uint8: umax8, \
    uint16: umax16, \
    uint32: umax32, \
    uint64: umax64  \
)(a, b)

#define min(a, b) _Generic((a), \
    int8: min8, \
    int16: min16, \
    int32: min32, \
    int64: min64, \
    uint8: umin8, \
    uint16: umin16, \
    uint32: umin32, \
    uint64: umin64  \
)(a, b)


uint8 umax8(uint8 a, uint8 b);
uint16 umax16(uint16 a, uint16 b);
uint32 umax32(uint32 a, uint32 b);
uint64 umax64(uint64 a, uint64 b);

int8 max8(int8 a, int8 b);
int16 max16(int16 a, int16 b);
int32 max32(int32 a, int32 b);
int64 max64(int64 a, int64 b);

uint8 umin8(uint8 a, uint8 b);
uint16 umin16(uint16 a, uint16 b);
uint32 umin32(uint32 a, uint32 b);
uint64 umin64(uint64 a, uint64 b);

int8 min8(int8 a, int8 b);
int16 min16(int16 a, int16 b);
int32 min32(int32 a, int32 b);
int64 min64(int64 a, int64 b);



#endif