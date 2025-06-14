#include "headers/vga.h"

#define   VGA_AC_INDEX      0x3C0
#define   VGA_AC_WRITE      0x3C0
#define   VGA_AC_READ         0x3C1
#define   VGA_MISC_WRITE      0x3C2
#define VGA_SEQ_INDEX      0x3C4
#define VGA_SEQ_DATA      0x3C5
#define   VGA_DAC_READ_INDEX   0x3C7
#define   VGA_DAC_WRITE_INDEX   0x3C8
#define   VGA_DAC_DATA      0x3C9
#define   VGA_MISC_READ      0x3CC
#define VGA_GC_INDEX       0x3CE
#define VGA_GC_DATA       0x3CF
#define VGA_CRTC_INDEX      0x3D4      /* 0x3B4 */
#define VGA_CRTC_DATA      0x3D5      /* 0x3B5 */
#define   VGA_INSTAT_READ      0x3DA
#define   VGA_NUM_SEQ_REGS   5
#define   VGA_NUM_CRTC_REGS   25
#define   VGA_NUM_GC_REGS      9
#define   VGA_NUM_AC_REGS      21
#define   VGA_NUM_REGS      (1+VGA_NUM_SEQ_REGS+VGA_NUM_CRTC_REGS+VGA_NUM_GC_REGS+VGA_NUM_AC_REGS)

uint8 mode13h[]={
         0x63,
    0x03,
    0x01,
    0x0F,
    0x00,
    0x0E,
    /* CRTC */
    0x5F, 0x4F, 0x50, 0x82, 0x54, 0x80, 0xBF, 0x1F,
    0x00, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x9C, 0x0E, 0x8F, 0x28, 0x40, 0x96, 0xB9, 0xA3,
    0xFF,
    /* GC */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x05, 0x0F,
    0xFF,
    /* AC */
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
    0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
    0x41, 0x00, 0x0F, 0x00,   0x00           
};


uint8 textmode[]={
  0x67,

  0x03,
  0x00,
  0x03,
  0x00,
  0x02,

  0x5B, 0x4F, 0x50, 0x82, 0x55, 0x81, 0xBF, 0x1F,
  0x00, 0x4F, 0x0D, 0x0E, 0x00, 0x00, 0x00, 0x50,
  0x9C, 0x0E, 0x8F, 0x28, 0x1F, 0x96, 0xB9, 0xA3,
  0xFF,

  0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x05, 0x0F,
  0xFF,


  0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x14, 0x07,
  0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
  0x0C, 0x00, 0x0F, 0x08, 0x00
};

void write_registers(uint8 *regs){
    uint32 i;
    
    /* write MISCELLANEOUS reg */
    outport(VGA_MISC_WRITE, *regs);
    regs++;
    /* write SEQUENCER regs */
    for(i = 0; i < VGA_NUM_SEQ_REGS; i++)
    {
        outport(VGA_SEQ_INDEX, i);
        outport(VGA_SEQ_DATA, *regs);
        regs++;
    }
    /* unlock CRTC registers */
    outport(VGA_CRTC_INDEX, 0x03);
    outport(VGA_CRTC_DATA, inport(VGA_CRTC_DATA) | 0x80);
    outport(VGA_CRTC_INDEX, 0x11);
    outport(VGA_CRTC_DATA, inport(VGA_CRTC_DATA) & ~0x80);
    /* make sure they remain unlocked */
    regs[0x03] |= 0x80;
    regs[0x11] &= ~0x80;
    /* write CRTC regs */
    for(i = 0; i < VGA_NUM_CRTC_REGS; i++)
    {
        outport(VGA_CRTC_INDEX, i);
        outport(VGA_CRTC_DATA, *regs);
        regs++;
    }
    /* write GRAPHICS CONTROLLER regs */
    for(i = 0; i < VGA_NUM_GC_REGS; i++)
    {
        outport(VGA_GC_INDEX, i);
        outport(VGA_GC_DATA, *regs);
        regs++;
    }
    /* write ATTRIBUTE CONTROLLER regs */
    for(i = 0; i < VGA_NUM_AC_REGS; i++)
    {
        (void)inport(VGA_INSTAT_READ);
        outport(VGA_AC_INDEX, i);
        outport(VGA_AC_WRITE, *regs);
        regs++;
    }
    
    /* lock 16-color palette and unblank display */
    (void)inport(VGA_INSTAT_READ);
    outport(VGA_AC_INDEX, 0x20);
}
uint8 backbuffer[320*200];
uint8 changed[320*200];
static uint8 *VGA_address;

void clear_screen(uint8 color){
  uint32 x,y;
  for(y=0; y<200; y++)
    for(x=0; x<320; x++)
      backbuffer[320*y+x]=VGA_address[320*y+x]=color;
}

void draw_pixel(uint32 x,uint32 y,uint8 color){
  uint32 i=y*320+x;
  if(i>320*200)
    return;
  changed[i]=1;
  backbuffer[i]=color;
}
//////// SFC - Simple Forms Rendering ///////////////////////


void lineLow(int32 x0,int32 y0,int32 x1,int32 y1,uint8 color){
int32 dx = x1 - x0;
int32 dy = y1 - y0;
int32 yi=1;
  if(dy<0){
   yi=-1;
   dy=-dy;
  }
  int32 D=(2*dy)-dx;
  int32 y=y0;
  for(int32 x=x0;x<x1;x++){
    draw_pixel(x,y,color);
    if(D>0){
      y=y+yi;
      D=D+(2*(dy-dx));
    }
    else{D=D+2*dy;}
  }
}

void lineHigh(int32 x0,int32 y0,int32 x1,int32 y1,uint8 color){
int32 dx = x1 - x0;
int32 dy = y1 - y0;
int32 xi=1;
  if(dx<0){
   xi=-1;
   dx=-dx;
  }
  int32 D=(2*dx)-dy;
  int32 x=x0;
  for(int32 y=y0;y<y1;y++){
    draw_pixel(x,y,color);
    if(D>0){
      x=x+xi;
      D=D+(2*(dx-dy));
    }
    else{D=D+2*dx;}
  }
}

int32 mathAbs(int32 n){
return (n<0)?-n:n;
}

void draw_line(int32 x0,int32 y0,int32 x1,int32 y1,uint8 color){
if(mathAbs(y1-y0)<mathAbs(x1-x0)){
  if(x0>x1)
    lineLow(x1,y1,x0,y0,color);
  else
    lineLow(x0,y0,x1,y1,color);
}
else {
  if(y0>y1)
    lineHigh(x1,y1,x0,y0,color);
  else 
    lineHigh(x0,y0,x1,y1,color);
}


}




///
void refresh(){
  uint32 x,y;
  for(y=0; y<200; y++)
    for(x=0; x<320; x++){
      int i=320*y+x;
      if(changed[i]){
	VGA_address[i]=backbuffer[i];
	changed[i]=0;
      }
    }
}
void initVGA(){
  VGA_address=(uint8*)0xA0000;
  write_registers(mode13h);
  clear_screen(0x0a);

draw_line(0,0,320,200,0x0f);

  refresh();
}

void initTextMode(){
write_registers(textmode);

}

