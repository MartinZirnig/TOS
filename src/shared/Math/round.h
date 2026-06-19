#if !defined(MATH_ROUND_H) && defined(__x86_64__)
#define MATH_ROUND_H

#include "types.h"

static inline int8   absi8(int8 x)   { return (x < 0) ? -x : x; }
static inline int16  absi16(int16 x) { return (x < 0) ? -x : x; }
static inline int32  absi32(int32 x) { return (x < 0) ? -x : x; }
static inline int64  absi64(int64 x) { return (x < 0) ? -x : x; }

static inline uint8  absu8(uint8 x)   { return x; }
static inline uint16 absu16(uint16 x) { return x; }
static inline uint32 absu32(uint32 x) { return x; }
static inline uint64 absu64(uint64 x) { return x; }

static inline float fabsf(float x) {
    union { float f; uint32 i; } u = { .f = x };
    u.i &= 0x7FFFFFFF;
    return u.f;
}

static inline double fabsd(double x) {
    union { double d; uint64 i; } u = { .d = x };
    u.i &= 0x7FFFFFFFFFFFFFFFULL;
    return u.d;
}

float  fceil(float x);
double dceil(double x);

float  ffloor(float x);
double dfloor(double x);

float  fround(float x);
double dround(double x);


#define abs(x) _Generic((x), \
    int8: absi8, int16: absi16, int32: absi32, int64: absi64, \
    uint8: absu8, uint16: absu16, uint32: absu32, uint64: absu64, \
    float: fabsf, double: fabsd \
)(x)

#define ceil(x) _Generic((x), \
    float: fceil, \
    double: dceil \
)(x)

#define floor(x) _Generic((x), \
    float: ffloor, \
    double: dfloor \
)(x)

#define round(x) _Generic((x), \
    float: fround, \
    double: dround \
)(x)

#endif