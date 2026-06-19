#ifndef DEBUG_H
#define DEBUG_H

#include "std.h"

void DEBUG_WriteChar(char character);
void DEBUG_WriteStr(str text); 
void DEBUG_WriteLine(str text); 
void DEBUG_NewLine();

void DEBUG_WriteInt8(int8 value);
void DEBUG_WriteInt16(int16 value);
void DEBUG_WriteInt32(int32 value);

void DEBUG_WriteUint8(uint8 value);
void DEBUG_WriteUint16(uint16 value);
void DEBUG_WriteUint32(uint32 value);

#if defined(__x86_64__)
void DEBUG_WriteInt64(int64 value);
void DEBUG_WriteUint64(uint64 value);
#endif


#define DEBUG_BREAK_POINT __asm__ volatile("int8")

#endif 
