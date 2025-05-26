#ifndef UTILITIES_H
#define UTILITIES_H
#include "types.h"

uint8 inport(uint16 port);

void outport(uint16 port,uint8 data);
void bflush();
char recordLastKey(uint8 input);
uint8 keyboard_read();
char getKeyboardKey(uint8 input);

#endif
