#if !defined(MATH_FUNCTIONS_H) && defined(__x86_64__)
#define MATH_FUNCTIONS_H

#include "types.h"


float  fsin(float x);
double dsin(double x);

float  fcos(float x);
double dcos(double x);

float  ftan(float x);
double dtan(double x);

float  fcotan(float x);
double dcotan(double x);


#define sin(x) _Generic((x), \
    float: fsin, \
    double: dsin \
)(x)

#define cos(x) _Generic((x), \
    float: fcos, \
    double: dcos \
)(x)

#define tan(x) _Generic((x), \
    float: ftan, \
    double: dtan \
)(x)

#define cotan(x) _Generic((x), \
    float: fcotan, \
    double: dcotan \
)(x)

#endif