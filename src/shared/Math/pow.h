#if !defined(MATH_POW_H) && defined(__x86_64__)
#define MATH_POW_H

#include "types.h"

static inline int8      sqri8(int8 x)       { return x * x; }
static inline int16     sqri16(int16 x)     { return x * x; }
static inline int32     sqri32(int32 x)     { return x * x; }
static inline int64     sqri64(int64 x)     { return x * x; }
static inline uint8     sqru8(uint8 x)      { return x * x; }
static inline uint16    sqru16(uint16 x)    { return x * x; }
static inline uint32    sqru32(uint32 x)    { return x * x; }
static inline uint64    sqru64(uint64 x)    { return x * x; }
static inline float     fsqr(float x)       { return x * x; }
static inline double    dsqr(double x)      { return x * x; }


static inline int64 pow_int_backend(int64 base, int64 exp) {
    int64 result = 1;
    if (exp < 0) return 0;
    while (exp) {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

static inline uint64 pow_uint_backend(uint64 base, uint64 exp) {
    uint64 result = 1;
    while (exp) {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

static inline int8   powi8(int8 b, int8 e)     { return (int8)pow_int_backend(b, e); }
static inline int16  powi16(int16 b, int16 e)   { return (int16)pow_int_backend(b, e); }
static inline int32  powi32(int32 b, int32 e)   { return (int32)pow_int_backend(b, e); }
static inline int64  powi64(int64 b, int64 e)   { return pow_int_backend(b, e); }
static inline uint8  powu8(uint8 b, uint8 e)    { return (uint8)pow_uint_backend(b, e); }
static inline uint16 powu16(uint16 b, uint16 e) { return (uint16)pow_uint_backend(b, e); }
static inline uint32 powu32(uint32 b, uint32 e) { return (uint32)pow_uint_backend(b, e); }
static inline uint64 powu64(uint64 b, uint64 e) { return pow_uint_backend(b, e); }



float     fpow(float base, float exp);
double    dpow(double base, double exp);

double    sqrti8(int8 x);
double    sqrti16(int16 x);
double    sqrti32(int32 x);
double    sqrti64(int64 x);
double    sqrtu8(uint8 x);
double    sqrtu16(uint16 x);
double    sqrtu32(uint32 x);
double    sqrtu64(uint64 x);
float     fsqrt(float x);
double    dsqrt(double x);

double    rooti8(int8 base, int8 exp);
double    rooti16(int16 base, int16 exp);
double    rooti32(int32 base, int32 exp);
double    rooti64(int64 base, int64 exp);
double    rootu8(uint8 base, uint8 exp);
double    rootu16(uint16 base, uint16 exp);
double    rootu32(uint32 base, uint32 exp);
double    rootu64(uint64 base, uint64 exp);
float     froot(float base, float exp);
double    droot(double base, double exp);


#define sqr(x) _Generic((x), \
    int8: sqri8, int16: sqri16, int32: sqri32, int64: sqri64, \
    uint8: sqru8, uint16: sqru16, uint32: sqru32, uint64: sqru64, \
    float: fsqr, double: dsqr \
)(x)

#define pow(base, exp) _Generic((base), \
    int8: powi8, int16: powi16, int32: powi32, int64: powi64, \
    uint8: powu8, uint16: powu16, uint32: powu32, uint64: powu64, \
    float: fpow, double: dpow \
)(base, exp)

#define sqrt(x) _Generic((x), \
    int8: sqrti8, int16: sqrti16, int32: sqrti32, int64: sqrti64, \
    uint8: sqrtu8, uint16: sqrtu16, uint32: sqrtu32, uint64: sqrtu64, \
    float: fsqrt, double: dsqrt \
)(x)

#define root(base, exp) _Generic((base), \
    int8: rooti8, int16: rooti16, int32: rooti32, int64: rooti64, \
    uint8: rootu8, uint16: rootu16, uint32: rootu32, uint64: rootu64, \
    float: froot, double: droot \
)(base, exp)

#endif