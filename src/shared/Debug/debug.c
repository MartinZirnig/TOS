#include "debug.h"
#include "std.h"

#define DEBUG_PORT      0xe9

static str NewLineSymbol = "\r\n";

void DEBUG_WriteChar(char character) {
    __out8__(DEBUG_PORT, character);
}

void DEBUG_WriteStr(char* text) {
    for (uint32 i = 0; text[i] != '\0'; i++)
        DEBUG_WriteChar(text[i]);
}
void DEBUG_NewLine() {
    DEBUG_WriteStr(NewLineSymbol);
}
void DEBUG_WriteLine(char* text) {
    DEBUG_WriteStr(text);
    DEBUG_NewLine();
}

void DEBUG_WriteInt8(int8 value) {
    char* text = itos(value);
    DEBUG_WriteStr(text);
    free(text);
}
void DEBUG_WriteInt16(int16 value) {
    char* text = itos(value);
    DEBUG_WriteStr(text);
    free(text);
}
void DEBUG_WriteInt32(int32 value) {
    char* text = itos(value);
    DEBUG_WriteStr(text);
    free(text);
}

void DEBUG_WriteUint8(uint8 value) {
    char* text = itos(value);
    DEBUG_WriteStr(text);
    free(text);
}
void DEBUG_WriteUint16(uint16 value) {
    char* text = itos(value);
    DEBUG_WriteStr(text);
    free(text);
}
void DEBUG_WriteUint32(uint32 value) {
    char* text = itos(value);
    DEBUG_WriteStr(text);
    free(text);
}

#if defined(__x86_64__)
void DEBUG_WriteInt64(int64 value) {
    char* text = itos(value);
    DEBUG_WriteStr(text);
    free(text);
}
void DEBUG_WriteUint64(uint64 value) {
    char* text = itos(value);
    DEBUG_WriteStr(text);
    free(text);
}
#endif