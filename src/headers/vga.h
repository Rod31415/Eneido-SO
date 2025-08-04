#ifndef VGA_H
#define VGA_H
#include "utilities.h"

typedef struct Color{
        uint8 Red;
        uint8 Green;
        uint8 Blue;
        uint8 Alpha;
}__attribute__((packed))Color_t;

typedef struct Palette8bits{
        Color Colors[256];
}__attribute__((packed))Palette8bits_t;


#define widthWindow 640
#define heightWindow 480
#define VGA ((uint8 *)0xA0000);

void initVGA(uint32 *fb,uint32 width,uint32 height);
void initTextMode();
uint32 get_pixel(uint32 x,uint32 y);
void draw_pixel(uint32 x,uint32 y,uint32 color);
void draw_line(int32 x0,int32 y0,int32 x1,int32 y1,uint32 color);
void clear_screen(uint32 color);
void draw_char(uint32 x,uint32 y,uint8 ch,uint32 color);
void draw_rect(uint32 x,uint32 y,uint32 width,uint32 height,uint32 color);

void changePalette(uint32 paletteAddress,bool array);
void change_ground_color(uint8 change);
void scrollDOWN(uint32 dist,uint8 c);
void refresh();
void refresh_rect(uint32 posx,uint32 posy,uint32 width,uint32 height);
#endif
