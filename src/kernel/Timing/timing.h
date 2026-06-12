#ifndef TIMING_H
#define TIMING_H

#include "std.h"

#define MAX_TIMERS 32

void delay(uint64 millis);
uint64 systemTime();

uint64 setTimeout(uint64 millis, void(*action)());
uint64 setInterval(uint64 millis, void(*action)());

void clearTimeout(uint64 timeoutId);
void clearInterval(uint64 intervalId);

void clearAllTimeouts();
void clearAllIntervals();

#endif