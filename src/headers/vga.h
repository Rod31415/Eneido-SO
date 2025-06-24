#ifndef VGA_H
#define VGA_H
#include "utilities.h"
#define VGA ((uint8 *)0xA0000);

void initVGA(uint8 *fb);
void initTextMode();
void draw_pixel(uint32 x,uint32 y,uint8 color);
void draw_line(int32 x0,int32 y0,int32 x1,int32 y1,uint8 color);
void clear_screen(uint8 color);
void draw_char(uint32 x,uint32 y,uint8 ch,uint8 color);
void draw_rect(uint32 x,uint32 y,uint32 width,uint32 height,uint8 color);
void change_ground_color(uint8 change);
void scrollDOWN(uint32 dist,uint8 c);
void refresh();
#endif
