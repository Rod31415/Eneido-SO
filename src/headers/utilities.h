#ifndef UTILITIES_H
#define UTILITIES_H
#include "types.h"

void setVGAMode(uint8 mode);
uint8 inport(uint16 port);
void memcpy(uint32 origin,uint32 destiny,uint32 size);
void memset(uint32 address,uint8 num,uint32 size);
void outport(uint16 port,uint8 data);
void bflush();
char recordLastKey(uint8 input);
uint8 keyboard_read();
uint8 getKeyboardKey(uint8 input);

#endif
