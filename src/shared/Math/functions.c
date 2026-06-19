#if defined(__x86_64__)

#include "functions.h"
#include "types.h"

#define TRIG_PI    3.14159265358979323846
#define TRIG_TWO_PI 6.28318530717958647692

static double reduce_angle(double x) {
    if (x < 0.0) {
        x = -x;
        long long v = (long long)(x / TRIG_TWO_PI);
        x = x - (double)v * TRIG_TWO_PI;
        return -x;
    } else {
        long long v = (long long)(x / TRIG_TWO_PI);
        x = x - (double)v * TRIG_TWO_PI;
    }
    
    if (x > TRIG_PI) {
        x -= TRIG_TWO_PI;
    }
    return x;
}

double dsin(double x) {
    x = reduce_angle(x);
    
    double x2 = x * x;
    double term = x;
    double sum = x;
    
    term = (term * x2) / 6.0;   sum -= term;
    term = (term * x2) / 20.0;  sum += term;
    term = (term * x2) / 42.0;  sum -= term;
    term = (term * x2) / 72.0;  sum += term;
    term = (term * x2) / 110.0; sum -= term;
    term = (term * x2) / 156.0; sum += term;
    
    return sum;
}

float fsin(float x) {
    return (float)dsin((double)x);
}

double dcos(double x) {
    x = reduce_angle(x);
    
    double x2 = x * x;
    double term = 1.0;
    double sum = 1.0;
    
    term = (term * x2) / 2.0;   sum -= term;
    term = (term * x2) / 12.0;  sum += term;
    term = (term * x2) / 30.0;  sum -= term;
    term = (term * x2) / 56.0;  sum += term;
    term = (term * x2) / 90.0;  sum -= term;
    term = (term * x2) / 132.0; sum += term;
    
    return sum;
}

float fcos(float x) {
    return (float)dcos((double)x);
}


double dtan(double x) {
    double c = dcos(x);
    if (c == 0.0) return DOUBLE_NAN;
    return dsin(x) / c;
}

float ftan(float x) {
    return (float)dtan((double)x);
}

double dcotan(double x) {
    double s = dsin(x);
    if (s == 0.0) return DOUBLE_NAN;
    return dcos(x) / s;
}

float fcotan(float x) {
    return (float)dcotan((double)x);
}

#endif