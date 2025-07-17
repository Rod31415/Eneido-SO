#ifndef UTILITIES_H
#define UTILITIES_H
#include "types.h"
#include "keyboard.h"


void setVGAMode(uint8 mode);
uint8 inport(uint16 port);
uint16 inportw(uint16 port);
uint32 inportl(uint16 port);
void memcpy(uint32 origin,uint32 destiny,uint32 size);
void memset(uint32 address,uint8 num,uint32 size);
void outport(uint16 port,uint8 data);
void outportw(uint16 port,uint16 data);
void outportl(uint16 port,uint32 data);

#endif
