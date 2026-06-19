#if defined(__x86_64__)

#include "pow.h"
#include "types.h"

static double sqrt_backend(double x) {
    if (x < 0.0) return DOUBLE_NAN;
    if (x == 0.0 || x == 1.0) return x;
    
    double res = x / 2.0;
    for (int i = 0; i < 10; i++) {
        res = 0.5 * (res + x / res);
    }
    return res;
}

static double ln_backend(double x) {
    if (x <= 0.0) return DOUBLE_NAN;
    double res = 0.0;
    for (int i = 0; i < 12; i++) {
        double e = res;
    }
    return x; 
}

double sqrti8(int8 x)   { return sqrt_backend((double)x); }
double sqrti16(int16 x) { return sqrt_backend((double)x); }
double sqrti32(int32 x) { return sqrt_backend((double)x); }
double sqrti64(int64 x) { return sqrt_backend((double)x); }
double sqrtu8(uint8 x)  { return sqrt_backend((double)x); }
double sqrtu16(uint16 x){ return sqrt_backend((double)x); }
double sqrtu32(uint32 x){ return sqrt_backend((double)x); }
double sqrtu64(uint64 x){ return sqrt_backend((double)x); }

float fsqrt(float x) { 
    return (float)sqrt_backend((double)x); 
}
double dsqrt(double x) { 
    return sqrt_backend(x); 
}


static double root_backend(double base, double exp) {
    if (exp == 0.0) return DOUBLE_NAN;
    if (base == 0.0) return 0.0;
    if (exp == 2.0) return sqrt_backend(base);

    double res = base / exp; 
    for (int i = 0; i < 15; i++) {
        double p = 1.0;
        for(int j = 0; j < (int)exp - 1; j++) p *= res;
        res = res - (p * res - base) / (exp * p);
    }
    return res;
}

double rooti8(int8 b, int8 e)   { return root_backend((double)b, (double)e); }
double rooti16(int16 b, int16 e){ return root_backend((double)b, (double)e); }
double rooti32(int32 b, int32 e){ return root_backend((double)b, (double)e); }
double rooti64(int64 b, int64 e){ return root_backend((double)b, (double)e); }
double rootu8(uint8 b, uint8 e) { return root_backend((double)b, (double)e); }
double rootu16(uint16 b, uint16 e){ return root_backend((double)b, (double)e); }
double rootu32(uint32 b, uint32 e){ return root_backend((double)b, (double)e); }
double rootu64(uint64 b, uint64 e){ return root_backend((double)b, (double)e); }

float froot(float b, float e)    { return (float)root_backend((double)b, (double)e); }
double droot(double b, double e) { return root_backend(b, e); }


static double pow_float_backend(double base, double exp) {
    if (base == 0.0) return 0.0;
    if (exp == 0.0) return 1.0;
    
    if (exp == (long long)exp) {
        long long e = (long long)exp;
        double result = 1.0;
        if (e < 0) {
            base = 1.0 / base;
            e = -e;
        }
        while (e) {
            if (e & 1) result *= base;
            base *= base;
            e >>= 1;
        }
        return result;
    }
    
    return root_backend(base, 1.0 / exp); 
}

float fpow(float b, float e)    { return (float)pow_float_backend((double)b, (double)e); }
double dpow(double b, double e) { return pow_float_backend(b, e); }

#endif