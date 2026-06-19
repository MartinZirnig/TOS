#include "types.h"

typedef struct
{
    dword address;
    word  width;
    word height;
    word pitch;
    word modeNumber;
} VesaContext;
