#ifndef VGA_H
#define VGA_H
#include "utilities.h"

#define VGA ((uint8 *)0xA0000);

void initVGA();
void initTextMode();
void draw_pixel(uint32 x,uint32 y,uint8 color);
void draw_line(uint32 x0,uint32 y0,uint32 x1,uint32 y1,uint8 color);
void clear_screen(uint8 color);
void refresh();
#endif
