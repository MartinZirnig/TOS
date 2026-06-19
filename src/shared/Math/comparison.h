#ifndef MATH_COMPARISON_H
#define MATH_COMPARISON_H

#include "types.h"


static inline uint8 umax8(uint8 a, uint8 b)     { return (a > b) ? a : b; }
static inline uint16 umax16(uint16 a, uint16 b) { return (a > b) ? a : b; }
static inline uint32 umax32(uint32 a, uint32 b) { return (a > b) ? a : b; }
static inline uint64 umax64(uint64 a, uint64 b) { return (a > b) ? a : b; }

static inline int8 max8(int8 a, int8 b)         { return (a > b) ? a : b; }
static inline int16 max16(int16 a, int16 b)     { return (a > b) ? a : b; }
static inline int32 max32(int32 a, int32 b)     { return (a > b) ? a : b; }
static inline int64 max64(int64 a, int64 b)     { return (a > b) ? a : b; }

static inline float fmax(float a, float b)             { return (a > b) ? a : b; }
static inline double dmax(double a, double b)          { return (a > b) ? a : b; }



static inline uint8 umin8(uint8 a, uint8 b)     { return (a < b) ? a : b; }
static inline uint16 umin16(uint16 a, uint16 b) { return (a < b) ? a : b; }
static inline uint32 umin32(uint32 a, uint32 b) { return (a < b) ? a : b; }
static inline uint64 umin64(uint64 a, uint64 b) { return (a < b) ? a : b; }

static inline int8 min8(int8 a, int8 b)         { return (a < b) ? a : b; }
static inline int16 min16(int16 a, int16 b)     { return (a < b) ? a : b; }
static inline int32 min32(int32 a, int32 b)     { return (a < b) ? a : b; }
static inline int64 min64(int64 a, int64 b)     { return (a < b) ? a : b; }

static inline float fmin(float a, float b)             { return (a < b) ? a : b; }
static inline double dmin(double a, double b)          { return (a < b) ? a : b; }



#define max(a, b) _Generic((a), \
    int8: max8, \
    int16: max16, \
    int32: max32, \
    int64: max64, \
    uint8: umax8, \
    uint16: umax16, \
    uint32: umax32, \
    uint64: umax64, \
    float: fmax, \
    double: dmax \
)(a, b)

#define min(a, b) _Generic((a), \
    int8: min8, \
    int16: min16, \
    int32: min32, \
    int64: min64, \
    uint8: umin8, \
    uint16: umin16, \
    uint32: umin32, \
    uint64: umin64, \
    float: fmin, \
    double: dmin \
)(a, b)

#endif