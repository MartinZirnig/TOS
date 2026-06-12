#ifndef STRING_OPERATIONS_H
#define STRING_OPERATIONS_H

#include "std.h"

uint32 str_len(char* text);
char* str_copy(char* text);
char* str_concat(char* first, char* next);
char* str_slice(char* text, uint32 start, uint32 len);
char* str_replaceC(char* text, char value, char replacement);
char* str_replaceS(char* text, char* value, char* replacement);

#endif