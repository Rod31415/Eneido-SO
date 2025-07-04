#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include "types.h"
#include "utilities.h"
#include "vga.h"


void sleep();
void cls(uint8 color);
void backspace();
void printDec(int32 dec);
void update_cursor(uint32 x, uint32 y);
void printHex(uint32 hex);
void printFlo(float flo);
void printBin(int32 bin);
void printChr(uint32 chr);
void changeColor(uint8 color);
void gotoxy(int32 x,int32 y);
void printf(const char* str, int32 arg0=0, int32 arg1=0, int32 arg2=0, int32 arg3=0, int32 arg4=0);
void resetTTYScreen();
void getConsoleCursorPosition(uint32 *x,uint32 *y);
void putChar(int32 x, int32 y, char character,uint8 color);
void nextLine();
void resetLine();
#endif
