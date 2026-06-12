#include "timing.h"

typedef struct {
    uint64 id;
    uint64 target_time;
    uint64 period;
    void (*action)();
} Timer;

static Timer timers[MAX_TIMERS];
static uint64 next_id = 1;

static volatile uint64 systemActiveMillis; 

static inline void _clearTimer(int index) {
    timers[index].id = 0;
    timers[index].target_time = 0;
    timers[index].period = 0;
    timers[index].action = null;
}

static void _updateTiming() {
    uint64 current_time = systemActiveMillis;

    for (int i = 0; i < MAX_TIMERS; i++) {
        if (timers[i].id == 0) continue; // Prázdný slot, ignorovat

        if (current_time >= timers[i].target_time) {
            void (*callback)() = timers[i].action;
            
            if (timers[i].period > 0) {
                timers[i].target_time = current_time + timers[i].period;
                callback(); 
            } else {
                _clearTimer(i);
                callback();
            }
        }
    }
}

static inline uint64 _addTimer(uint64 millis, void(*action)(), uint64 period) {
    if (!action) return 0;

    for (int i = 0; i < MAX_TIMERS; i++) {
        if (timers[i].id == 0) {
            timers[i].id = next_id++;
            timers[i].target_time = systemActiveMillis + millis;
            timers[i].period = period;
            timers[i].action = action;
            return timers[i].id;
        }
    }
    return 0;
}



nuint ___timing_system_active_millis___() {
    return (nuint)&systemActiveMillis;
}


uint64 systemTime() {
    return systemActiveMillis;
}

void delay(uint64 millis) {
    uint64 start = systemActiveMillis;
    while ((systemActiveMillis - start) < millis) {
        asm volatile("nop");
    }
}

uint64 setTimeout(uint64 millis, void(*action)()) {
    return _addTimer(millis, action, 0);
}

uint64 setInterval(uint64 millis, void(*action)()) {
    return _addTimer(millis, action, millis);
}

void clearTimeout(uint64 timeoutId) {
    if (timeoutId == 0) return;
    for (int i = 0; i < MAX_TIMERS; i++) {
        if (timers[i].id == timeoutId && timers[i].period == 0) {
            _clearTimer(i);
            return;
        }
    }
}

void clearInterval(uint64 intervalId) {
    if (intervalId == 0) return;
    for (int i = 0; i < MAX_TIMERS; i++) {
        if (timers[i].id == intervalId && timers[i].period > 0) {
            _clearTimer(i);
            return;
        }
    }
}

void clearAllTimeouts() {
    for (int i = 0; i < MAX_TIMERS; i++) {
        if (timers[i].id != 0 && timers[i].period == 0) {
            _clearTimer(i);
        }
    }
}

void clearAllIntervals() {
    for (int i = 0; i < MAX_TIMERS; i++) {
        if (timers[i].id != 0 && timers[i].period > 0) {
            _clearTimer(i);
        }
    }
}