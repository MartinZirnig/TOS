#include "VGA.h"
#include "HW_IO.h"
#include "memory.h"

#define VGA_LINE_BUFFER_SIZE        VGA_COLUMNS * sizeof(uint16)

static volatile nuint VGA_BOTTOM = none;
static volatile nuint VGA_LIMIT = none;

static volatile uint16* CurrentPosition = null;
static volatile VGA_Color Background = VGA_COLOR_BLACK;
static volatile VGA_Color Foreground = VGA_COLOR_WHITE;
static volatile bool Blink = 0;


static inline void _updateHardwareCursor() {
    nuint pos = ((nuint)CurrentPosition - (nuint)VGA_BOTTOM) / 2;

    __out8__(0x3D4, 14);
    __out8__(0x3D5, (byte)((pos >> 8) & 0xFF));

    __out8__(0x3D4, 15);
    __out8__(0x3D5, (byte)(pos & 0xFF));
}

static inline void _printCharacter(uint16 character) {
    while ((nuint)CurrentPosition >= (nuint)VGA_LIMIT) {
        VGA_Scroll();
    }
        
    CurrentPosition[0] = character;
    CurrentPosition += 1;

    _updateHardwareCursor();
}


void VGA_Scroll() {
    uint16* vga_buffer = (uint16*)VGA_BOTTOM;
    
    uint16* dest = vga_buffer;
    uint16* src = vga_buffer + VGA_COLUMNS;
    uint32 bytesToCopy = (VGA_LINES - 1) * VGA_COLUMNS * sizeof(uint16);
    
    copy(src, dest, bytesToCopy);

    uint16* lastLine = vga_buffer + ((VGA_LINES - 1) * VGA_COLUMNS);
    fill(lastLine, VGA_COLUMNS * 2, UINT32_MIN);

    byte y = VGA_GetCursorY() - 1;
    if (y < 0) y = 0;
    
    VGA_SetCursorPosition(VGA_GetCursorX(), y);
}

void VGA_Initialize(void* bot, void* limit) {
    VGA_BOTTOM = (nuint)bot;
    VGA_LIMIT = (nuint)limit;
    
    VGA_Reset();
}

void VGA_Write(char* text) {
    int index = 0;
    while (1) {
        char current = text[index++];
        if (current == 0)
            break;
        
        uint16 transformed = VGA_TransformCharacter(current, Foreground, Background, Blink);
        _printCharacter(transformed);
    }

}

void VGA_WriteRaw(uint16* text) {
    int index = 0;
    while (1) {
        uint16 current = text[index++];
        if (current == 0)
            break;
        
        _printCharacter(current);
    }
}

uint16 VGA_TransformCharacter(char character, VGA_Color foreground, VGA_Color background, bool blink) {
    uint16 transformedChar = character; 
    uint16 transformedForeground = (foreground & 0x0F) << 8;
    uint16 transformedBackground = (background & 0x07) << 12;
    uint16 transformedBlink = (blink & 0x1) << 15;

    return transformedChar | transformedForeground | transformedBackground | transformedBlink;
}

void VGA_SetCursorPosition(byte x, byte y) {
    uint16 pos = y * 80 + x;

    CurrentPosition = (uint16*)VGA_BOTTOM;
    CurrentPosition += pos;

    _updateHardwareCursor();
}

byte VGA_GetCursorX() {
    nuint pos = ((nuint)CurrentPosition - VGA_BOTTOM) / 2;
    return (byte)(pos % 80);
}

byte VGA_GetCursorY() {
    nuint pos = ((nuint)CurrentPosition - VGA_BOTTOM) / 2;
    return (byte)(pos / 80);
}

inline void VGA_NewLine() {
    VGA_SetCursorPosition(0, VGA_GetCursorY() + 1);
}

void VGA_SetForegroundColor(VGA_Color color) {
    Foreground = color;
}

void VGA_SetBackgroundColor(VGA_Color color) {
    Background = color;
}

void VGA_SetBlink(bool blink) {
    Blink = blink;
}

void VGA_Clear() {
    CurrentPosition = (uint16*)VGA_BOTTOM;
    uint16 blank = VGA_TransformCharacter(' ', Foreground, Background, Blink);

    while ((nuint)CurrentPosition < VGA_LIMIT) {
        _printCharacter(blank);
    }

    CurrentPosition = (uint16*)VGA_BOTTOM;
}

void VGA_Reset() {
    Foreground = VGA_COLOR_WHITE;
    Background = VGA_COLOR_BLACK;
    Blink = 0;
    VGA_Clear();
}