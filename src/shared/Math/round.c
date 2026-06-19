#if defined(__x86_64__)

#include "round.h"
#include "types.h"

#define FLOAT_INTEGRAL_LIMIT  8388608.0f
#define DOUBLE_INTEGRAL_LIMIT 4503599627370496.0


float fceil(float x) {
    union { float f; uint32 i; } u = { .f = x };
    uint32 sign = u.i & 0x80000000;
    
    if (fabsf(x) >= FLOAT_INTEGRAL_LIMIT || x != x) {
        return x;
    }

    int32 truncated = (int32)x;
    float truncated_f = (float)truncated;

    if (truncated_f < x && !sign) {
        return truncated_f + 1.0f;
    }
    return truncated_f;
}

double dceil(double x) {
    union { double d; uint64 i; } u = { .d = x };
    uint64 sign = u.i & 0x8000000000000000ULL;

    if (fabsd(x) >= DOUBLE_INTEGRAL_LIMIT || x != x) {
        return x;
    }

    int64 truncated = (int64)x;
    double truncated_d = (double)truncated;

    if (truncated_d < x && !sign) {
        return truncated_d + 1.0;
    }
    return truncated_d;
}


float ffloor(float x) {
    union { float f; uint32 i; } u = { .f = x };
    uint32 sign = u.i & 0x80000000;

    if (fabsf(x) >= FLOAT_INTEGRAL_LIMIT || x != x) {
        return x;
    }

    int32 truncated = (int32)x;
    float truncated_f = (float)truncated;

    if (truncated_f > x && sign) {
        return truncated_f - 1.0f;
    }
    return truncated_f;
}

double dfloor(double x) {
    union { double d; uint64 i; } u = { .d = x };
    uint64 sign = u.i & 0x8000000000000000ULL;

    if (fabsd(x) >= DOUBLE_INTEGRAL_LIMIT || x != x) {
        return x;
    }

    int64 truncated = (int64)x;
    double truncated_d = (double)truncated;

    if (truncated_d > x && sign) {
        return truncated_d - 1.0;
    }
    return truncated_d;
}

float fround(float x) {
    if (fabsf(x) >= FLOAT_INTEGRAL_LIMIT || x != x) {
        return x;
    }

    union { float f; uint32 i; } u = { .f = x };
    uint32 sign = u.i & 0x80000000;

    if (sign) {
        float t = x - 0.5f;
        int32 truncated = (int32)t;
        if ((float)truncated > t) return (float)truncated;
        return (float)truncated;
    } else {
        float t = x + 0.5f;
        int32 truncated = (int32)t;
        if ((float)truncated < t) return (float)truncated;
        return (float)truncated;
    }
}

double dround(double x) {
    if (fabsd(x) >= DOUBLE_INTEGRAL_LIMIT || x != x) {
        return x;
    }

    union { double d; uint64 i; } u = { .d = x };
    uint64 sign = u.i & 0x8000000000000000ULL;

    if (sign) {
        double t = x - 0.5;
        int64 truncated = (int64)t;
        if ((double)truncated > t) return (double)truncated;
        return (double)truncated;
    } else {
        double t = x + 0.5;
        int64 truncated = (int64)t;
        if ((double)truncated < t) return (double)truncated;
        return (double)truncated;
    }
}

#endif