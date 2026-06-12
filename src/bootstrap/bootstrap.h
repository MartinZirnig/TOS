#ifndef BTS_OUT_H
#define BTS_OUT_H

#include "std.h"
#include "string.h"
#include "memory.h"
#include "addr.h"

uint64 div64(uint64 value, uint64 divider);
uint64 mod64(uint64 value, uint64 modus);

str uint64ToString(uint64 value);
void print(str message);
void printInfo(str message);
void printSuccess(str message);
void printError(str message);

void sleep(uint32 millis);

#endif