#include "bootstrap.h"
#include "VGA.h"

str uint64ToString(uint64 value)
{
    char buffer[21];
    int pos = 20;

    buffer[pos] = '\0';

    do
    {
        buffer[--pos] = '0' + (char)mod64(value, 10);;
        value = div64(value, 10);
    }
    while (value > 0);

    int len = 20 - pos;

    str result = malloc(len + 1);
    if (!result)
        return null;

    copy(&buffer[pos], result, len + 1);

    return result;
}

void print(char* message) {
    VGA_SetForegroundColor(VGA_COLOR_WHITE);
    VGA_Write(message);
    VGA_NewLine();

    sleep(50);
}

void printError(char* message) {
    VGA_SetForegroundColor(VGA_COLOR_RED);
    VGA_Write("Error: ");
    VGA_Write(message);
    VGA_SetForegroundColor(VGA_COLOR_WHITE);
    VGA_NewLine();

    sleep(500);
}

void printSuccess(char* message) {
    VGA_SetForegroundColor(VGA_COLOR_GREEN);
    VGA_Write("Success: ");
    VGA_Write(message);
    VGA_SetForegroundColor(VGA_COLOR_WHITE);
    VGA_NewLine();

    sleep(500);
}

void printInfo(char* message) {
    VGA_SetForegroundColor(VGA_COLOR_LIGHT_YELLOW);
    VGA_Write("Info: ");
    VGA_Write(message);
    VGA_SetForegroundColor(VGA_COLOR_WHITE);
    VGA_NewLine();
    
    sleep(500);
}