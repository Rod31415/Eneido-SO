#ifndef UTILITIES_H
#define UTILITIES_H
#include "types.h"


#define   enterKeyCode 128
#define   retKeyCode 129
#define  downKeyCode 130
#define  leftKeyCode 131
#define    upKeyCode 132
#define rightKeyCode 133
#define   escKeyCode 135

void setVGAMode(uint8 mode);
uint8 inport(uint16 port);
uint16 inportw(uint16 port);
uint32 inportl(uint16 port);
void memcpy(uint32 origin,uint32 destiny,uint32 size);
void memset(uint32 address,uint8 num,uint32 size);
void outport(uint16 port,uint8 data);
void outportl(uint16 port,uint32 data);
void bflush();
char recordLastKey(uint8 input);
uint8 keyboard_read();
uint8 getKeyboardKey(uint8 input);

#endif
