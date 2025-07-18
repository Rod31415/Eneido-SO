#include "headers/vga.h"
#define VGA_AC_INDEX 0x3C0
#define VGA_AC_WRITE 0x3C0
#define VGA_AC_READ 0x3C1
#define VGA_MISC_WRITE 0x3C2
#define VGA_SEQ_INDEX 0x3C4
#define VGA_SEQ_DATA 0x3C5
#define VGA_DAC_READ_INDEX 0x3C7
#define VGA_DAC_WRITE_INDEX 0x3C8
#define VGA_DAC_DATA 0x3C9
#define VGA_MISC_READ 0x3CC
#define VGA_GC_INDEX 0x3CE
#define VGA_GC_DATA 0x3CF
#define VGA_CRTC_INDEX 0x3D4 /* 0x3B4 */
#define VGA_CRTC_DATA 0x3D5  /* 0x3B5 */
#define VGA_INSTAT_READ 0x3DA
#define VGA_NUM_SEQ_REGS 5
#define VGA_NUM_CRTC_REGS 25
#define VGA_NUM_GC_REGS 9
#define VGA_NUM_AC_REGS 21
#define VGA_NUM_REGS (1 + VGA_NUM_SEQ_REGS + VGA_NUM_CRTC_REGS + VGA_NUM_GC_REGS + VGA_NUM_AC_REGS)

#define VIDEO_SIZE 38400

unsigned char Fonts[2048] = {
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x18,0x3c,0x3c,0x18,0x18,0x00,0x18,0x00,
0x66,0x66,0x24,0x00,0x00,0x00,0x00,0x00,
0x6c,0x6c,0xfe,0x6c,0xfe,0x6c,0x6c,0x00,
0x30,0x7c,0xc0,0x78,0x0c,0xf8,0x30,0x00,
0x62,0x96,0x6c,0x18,0x34,0x6a,0xc4,0x00,
0x38,0x6c,0x38,0x76,0xdc,0xcc,0x76,0x00,
0x60,0x60,0xc0,0x00,0x00,0x00,0x00,0x00,
0x30,0x60,0xc0,0xc0,0xc0,0x60,0x30,0x00,
0xc0,0x60,0x30,0x30,0x30,0x60,0xc0,0x00,
0x00,0x66,0x3c,0xff,0x3c,0x66,0x00,0x00,
0x00,0x30,0x30,0xfc,0x30,0x30,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x30,0x30,0x60,
0x00,0x00,0x00,0xfc,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x30,0x30,0x00,
0x06,0x0c,0x18,0x30,0x60,0xc0,0x80,0x00,
0x78,0xcc,0xdc,0xfc,0xec,0xcc,0x78,0x00,
0x30,0x70,0xf0,0x30,0x30,0x30,0xfc,0x00,
0x78,0xcc,0x0c,0x38,0x60,0xcc,0xfc,0x00,
0x78,0xcc,0x0c,0x38,0x0c,0xcc,0x78,0x00,
0x18,0x38,0x78,0xd8,0xfc,0x18,0x3c,0x00,
0xfc,0xc0,0xf8,0x0c,0x0c,0x8c,0x78,0x00,
0x38,0x60,0xc0,0xf8,0xcc,0xcc,0x78,0x00,
0xfc,0xcc,0x0c,0x18,0x30,0x30,0x30,0x00,
0x78,0xcc,0xcc,0x78,0xcc,0xcc,0x78,0x00,
0x78,0xcc,0xcc,0x7c,0x0c,0x18,0x70,0x00,
0x00,0x30,0x30,0x00,0x00,0x30,0x30,0x00,
0x00,0x30,0x30,0x00,0x30,0x30,0x60,0x00,
0x18,0x30,0x60,0xc0,0x60,0x30,0x18,0x00,
0x00,0x00,0xfe,0x00,0xfe,0x00,0x00,0x00,
0x60,0x30,0x18,0x0c,0x18,0x30,0x60,0x00,
0x78,0xcc,0x0c,0x18,0x30,0x00,0x30,0x00,
0x7c,0xc6,0xde,0xde,0xde,0xc0,0x78,0x00,
0x3c,0x66,0x66,0x7e,0x66,0x66,0x66,0x00,
0xfc,0x66,0x66,0x7c,0x66,0x66,0xfc,0x00,
0x7c,0xc6,0xc0,0xc0,0xc0,0xc6,0x7c,0x00,
0xfc,0x66,0x66,0x66,0x66,0x66,0xfc,0x00,
0xfe,0x62,0x68,0x78,0x68,0x62,0xfe,0x00,
0xfe,0x62,0x68,0x78,0x68,0x60,0xf0,0x00,
0x7c,0xc6,0xc0,0xc0,0xce,0xc6,0x7e,0x00,
0x66,0x66,0x66,0x7e,0x66,0x66,0x66,0x00,
0x3c,0x18,0x18,0x18,0x18,0x18,0x3c,0x00,
0x1e,0x0c,0x0c,0x0c,0xcc,0xcc,0x78,0x00,
0xf6,0x66,0x6c,0x78,0x6c,0x66,0xf6,0x00,
0xf0,0x60,0x60,0x60,0x60,0x62,0xfe,0x00,
0xc6,0xee,0xfe,0xd6,0xc6,0xc6,0xc6,0x00,
0xcc,0xec,0xec,0xfc,0xdc,0xdc,0xcc,0x00,
0x7c,0xc6,0xc6,0xc6,0xc6,0xc6,0x7c,0x00,
0xfc,0x66,0x66,0x7c,0x60,0x60,0xf0,0x00,
0x7c,0xc6,0xc6,0xc6,0xc6,0xde,0x7c,0x0e,
0xfc,0x66,0x66,0x78,0x6c,0x66,0xf6,0x00,
0x78,0xcc,0xe0,0x78,0x1c,0xcc,0x78,0x00,
0xfc,0xb4,0x30,0x30,0x30,0x30,0x78,0x00,
0xcc,0xcc,0xcc,0xcc,0xcc,0xcc,0x78,0x00,
0xcc,0xcc,0xcc,0xcc,0xcc,0x78,0x30,0x00,
0xc6,0xc6,0xc6,0xd6,0xfe,0xee,0xc6,0x00,
0xcc,0xcc,0x78,0x30,0x78,0xcc,0xcc,0x00,
0xcc,0xcc,0xcc,0x78,0x30,0x30,0x78,0x00,
0xfc,0xcc,0x98,0x30,0x64,0xcc,0xfc,0x00,
0x78,0x60,0x60,0x60,0x60,0x60,0x78,0x00,
0xc0,0x60,0x30,0x18,0x0c,0x06,0x02,0x00,
0x78,0x18,0x18,0x18,0x18,0x18,0x78,0x00,
0x10,0x38,0x6c,0xc6,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,
0x30,0x30,0x18,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x78,0x0c,0x7c,0xcc,0x76,0x00,
0xe0,0x60,0x7c,0x66,0x66,0x66,0xdc,0x00,
0x00,0x00,0x78,0xcc,0xc0,0xcc,0x78,0x00,
0x1c,0x0c,0x7c,0xcc,0xcc,0xcc,0x76,0x00,
0x00,0x00,0x3c,0x66,0x7e,0x60,0x3c,0x00,
0x1c,0x36,0x30,0x78,0x30,0x30,0x78,0x00,
0x00,0x00,0x76,0xcc,0xcc,0x7c,0x0c,0x78,
0xe0,0x60,0x7c,0x66,0x66,0x66,0xf6,0x00,
0x30,0x00,0x70,0x30,0x30,0x30,0x78,0x00,
0x0c,0x00,0x1c,0x0c,0x0c,0xcc,0xcc,0x78,
0xe0,0x66,0x6c,0x78,0x6c,0x66,0xf6,0x00,
0x70,0x30,0x30,0x30,0x30,0x30,0x78,0x00,
0x00,0x00,0xec,0xda,0xda,0xda,0xda,0x00,
0x00,0x00,0xdc,0x66,0x66,0x66,0x66,0x00,
0x00,0x00,0x78,0xcc,0xcc,0xcc,0x78,0x00,
0x00,0x00,0xdc,0x66,0x66,0x7c,0x60,0xf0,
0x00,0x00,0x76,0xcc,0xcc,0x7c,0x0c,0x1e,
0x00,0x00,0xdc,0x76,0x60,0x60,0xf0,0x00,
0x00,0x00,0x7c,0xe0,0x7c,0x0e,0xfc,0x00,
0x20,0x60,0xf8,0x60,0x60,0x64,0x38,0x00,
0x00,0x00,0xcc,0xcc,0xcc,0xcc,0x76,0x00,
0x00,0x00,0xcc,0xcc,0xcc,0x78,0x30,0x00,
0x00,0x00,0xc6,0xd6,0xfe,0xee,0xc6,0x00,
0x00,0x00,0xcc,0x78,0x30,0x78,0xcc,0x00,
0x00,0x00,0xcc,0xcc,0xcc,0x7c,0x0c,0x78,
0x00,0x00,0xfc,0x98,0x30,0x64,0xfc,0x00,
0x1c,0x30,0x30,0xe0,0x30,0x30,0x1c,0x00,
0x18,0x18,0x18,0x00,0x18,0x18,0x18,0x00,
0xe0,0x30,0x30,0x1c,0x30,0x30,0xe0,0x00,
0x76,0xdc,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
};

uint8 mode = 0;
unsigned char mode12h[] =
    {
	/* MISC */
	0xE3,
	/* SEQ */
	0x03, 0x01, 0x08, 0x00, 0x06,
	/* CRTC */
	0x5F, 0x4F, 0x50, 0x82, 0x54, 0x80, 0x0B, 0x3E,
	0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0xEA, 0x0C, 0xDF, 0x28, 0x00, 0xE7, 0x04, 0xE3,
	0xFF,
	/* GC */
	0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x05, 0x0F,
	0xFF,
	/* AC */
	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x14, 0x07,
	0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
	0x01, 0x00, 0x0F, 0x00, 0x00};
uint8 mode13h[] = {
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
    0x41, 0x00, 0x0F, 0x00, 0x00};

uint8 textmode[] = {
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
    0x0C, 0x00, 0x0F, 0x08, 0x00};

void write_registers(uint8 *regs)
{
  uint32 i;

  /* write MISCELLANEOUS reg */
  outport(VGA_MISC_WRITE, *regs);
  regs++;
  /* write SEQUENCER regs */
  for (i = 0; i < VGA_NUM_SEQ_REGS; i++)
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
  for (i = 0; i < VGA_NUM_CRTC_REGS; i++)
  {
    outport(VGA_CRTC_INDEX, i);
    outport(VGA_CRTC_DATA, *regs);
    regs++;
  }
  /* write GRAPHICS CONTROLLER regs */
  for (i = 0; i < VGA_NUM_GC_REGS; i++)
  {
    outport(VGA_GC_INDEX, i);
    outport(VGA_GC_DATA, *regs);
    regs++;
  }
  /* write ATTRIBUTE CONTROLLER regs */
  for (i = 0; i < VGA_NUM_AC_REGS; i++)
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

uint32 WIDTH = 0;
uint32 HEIGHT = 0;

void refresh();

uint8 backbuffer[widthWindow * heightWindow];
uint8 changed[widthWindow * heightWindow];
volatile uint8 *VGA_address = (volatile uint8 *)0xA0000;

uint8 CHANGE_GROUND_COLOR = 1;

static const uint8 masks[8] = {
    0x80, 0x40, 0x20, 0x10,
    0x08, 0x04, 0x02, 0x01};

void scrollDOWN(uint32 dist, uint8 c)
{
  for (uint32 i = 0; i < WIDTH * HEIGHT - WIDTH * dist; i++)
  {
    backbuffer[i] = backbuffer[i + WIDTH * dist];
    changed[i] = 1;
  }
  for (uint32 i = 0; i < WIDTH * dist; i++)
  {
    backbuffer[i + WIDTH * HEIGHT - WIDTH * dist] = 0x00;
    changed[i + WIDTH * HEIGHT - WIDTH * dist] = 1;
  }
}

void scrollUP(uint32 dist)
{
  /*for(uint32 i=0;i<320*200-320;i++){
    backbuffer[i]=VGA_address[i+200];
  }
  for(uint32 i=0;i<320;i++){
    backbuffer[i+320*200-320]=color;
  }
  */
}

void set_plane(uint8 plane)
{
  outport(0x3c4, 0x02);
  outport(0x3c5, 1 << (plane & 3));
}

void draw_pixel(uint32 x, uint32 y, uint8 color)
{
  if (mode == 12)
  {
    uint32 index = y * (640 / 8) + (x / 8);
    uint8 bitmask = masks[x % 8];

    for (uint8 plane = 0; plane < 4; ++plane)
    {
      set_plane(plane);
      if (color & (1 << plane))
      {
	VGA_address[index] |= bitmask;
      }
      else
      {
	VGA_address[index] &= ~bitmask;
      }
    }
  }
  else if (mode == 13)
  {
    uint32 i = y * WIDTH + x;
    if (i > WIDTH * HEIGHT)
      return;
    changed[i] = 1;
    backbuffer[i] = color;
  }
}

void clear_screen(uint8 color)
{
  if (mode == 12)
  {

    for (uint8 plane = 0; plane < 4; ++plane)
    {
      set_plane(plane);
      // uint8 fill = (color & (1 << plane)) ? 0xFF : 0x00;
      for (uint32 i = 0; i < 38400; ++i)
      {
      }
    }
  }
  else if (mode == 13)
  {
    for (uint32 y = 0; y < HEIGHT; y++)
      for (uint32 x = 0; x < WIDTH; x++)
	backbuffer[WIDTH * y + x] = VGA_address[WIDTH * y + x] = 0x00;
  }
}

//////// SFC - Simple Forms Rendering ///////////////////////

void lineLow(int32 x0, int32 y0, int32 x1, int32 y1, uint8 color)
{
  int32 dx = x1 - x0;
  int32 dy = y1 - y0;
  int32 yi = 1;
  if (dy < 0)
  {
    yi = -1;
    dy = -dy;
  }
  int32 D = (2 * dy) - dx;
  int32 y = y0;
  for (int32 x = x0; x < x1; x++)
  {
    draw_pixel(x, y, color);
    if (D > 0)
    {
      y = y + yi;
      D = D + (2 * (dy - dx));
    }
    else
    {
      D = D + 2 * dy;
    }
  }
}

void lineHigh(int32 x0, int32 y0, int32 x1, int32 y1, uint8 color)
{
  int32 dx = x1 - x0;
  int32 dy = y1 - y0;
  int32 xi = 1;
  if (dx < 0)
  {
    xi = -1;
    dx = -dx;
  }
  int32 D = (2 * dx) - dy;
  int32 x = x0;
  for (int32 y = y0; y < y1; y++)
  {
    draw_pixel(x, y, color);
    if (D > 0)
    {
      x = x + xi;
      D = D + (2 * (dx - dy));
    }
    else
    {
      D = D + 2 * dx;
    }
  }
}

int32 mathAbs(int32 n)
{
  return (n < 0) ? -n : n;
}

void draw_line(int32 x0, int32 y0, int32 x1, int32 y1, uint8 color)
{
  if (mathAbs(y1 - y0) < mathAbs(x1 - x0))
  {
    if (x0 > x1)
      lineLow(x1, y1, x0, y0, color);
    else
      lineLow(x0, y0, x1, y1, color);
  }
  else
  {
    if (y0 > y1)
      lineHigh(x1, y1, x0, y0, color);
    else
      lineHigh(x0, y0, x1, y1, color);
  }
}

void draw_rect(uint32 x, uint32 y, uint32 width, uint32 height, uint8 color)
{
  for (uint32 hI = 0; hI < height; hI++)
  {
    uint32 i = (y + hI) * WIDTH + x;
    if (i + width > WIDTH * HEIGHT)
      return;
    for (uint32 wI = 0; wI < width; wI++)
    {
      changed[i + wI] = 1;
      backbuffer[i + wI] = color;
    }
  }
}

void change_ground_color(uint8 change)
{
  CHANGE_GROUND_COLOR = change;
}

void draw_char(uint32 x, uint32 y, uint8 ch, uint8 color)
{
  uint8 *glyph = Fonts + (ch * 8);
  uint8 selected = ch % 2;
  for (uint32 i = 0; i < 8; i++)
  {
    uint8 row = glyph[i];
    for (uint32 j = 0; j < 8; j++)
    {
      if (row & (0x80 >> j))
      {
	draw_pixel(x + j, y + i * 2, color);
	draw_pixel(x + j, y + i * 2 + 1, color);
      }
      else if (CHANGE_GROUND_COLOR)
      {
	draw_pixel(x + j, y + i * 2, 0x00);
	draw_pixel(x + j, y + i * 2 + 1, 0x00);
      }
    }
  }
}

///
void refresh()
{

  uint32 x, y;
  for (y = 0; y < HEIGHT; y++)
    for (x = 0; x < WIDTH; x++)
    {
      int i = WIDTH * y + x;
      if (changed[i])
      {
	VGA_address[i] = backbuffer[i];
	changed[i] = 0;
      }
    }
}
void initVGA(uint8 *fb, uint32 width, uint32 height)
{
  VGA_address = (volatile uint8 *)fb;

  mode = 13;
  WIDTH = width;
  HEIGHT = height;
  clear_screen(2);
}

void initTextMode()
{
  write_registers(textmode);
}
