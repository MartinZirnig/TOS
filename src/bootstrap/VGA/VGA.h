#ifndef VGA_H
#define VGA_H

#include "std.h"

#define VGA_LINES               25
#define VGA_COLUMNS             80

typedef enum VGA_Color {
    VGA_COLOR_BLACK = 0x0,
    VGA_COLOR_BLUE = 0x1,
    VGA_COLOR_GREEN = 0x2,
    VGA_COLOR_CYAN = 0x3,
    VGA_COLOR_RED = 0x4,
    VGA_COLOR_MAGENTA = 0x5,
    VGA_COLOR_BROWN = 0x6,

    VGA_COLOR_LIGHT_GREY = 0x7,
    VGA_COLOR_DARK_GREY = 0x8,
    VGA_COLOR_LIGHT_BLUE = 0x9,
    VGA_COLOR_LIGHT_GREEN = 0xA,
    VGA_COLOR_LIGHT_CYAN = 0xB,
    VGA_COLOR_LIGHT_RED = 0xC,
    VGA_COLOR_LIGHT_MAGENTA = 0xD,
    VGA_COLOR_LIGHT_YELLOW = 0xE,

    VGA_COLOR_WHITE = 0xF,
} VGA_Color;

void VGA_Initialize(void* bot, void* limit);

void VGA_Write(char* text);
void VGA_WriteRaw(uint16* text);
void VGA_SetCursorPosition(byte x, byte y);
byte VGA_GetCursorX();
byte VGA_GetCursorY();
void VGA_NewLine();
void VGA_Scroll();
void VGA_SetForegroundColor(enum VGA_Color color);
void VGA_SetBackgroundColor(enum VGA_Color color);
uint16 VGA_TransformCharacter(char character, enum VGA_Color foreground, enum VGA_Color background, bool blink);
void VGA_SetBlink(bool blink);
void VGA_Clear();
void VGA_Reset();

#endif