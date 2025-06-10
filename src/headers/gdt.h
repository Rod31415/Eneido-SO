#ifndef GDT_H
#define GDT_H
#include "types.h"


typedef struct{
 uint16 LimitLO;
 uint16 BaseLO;
 uint8  BaseME;
 uint8  AccessByte;
 uint8  FlsLimitHI;
 uint8  BaseHI;

}__attribute__((packed))GDT_Entry;



struct gdt_ptr
{
    uint16 limit;
    uint32 base;
} __attribute__((packed));


void gdt_set_gate(int num, unsigned long base, unsigned long limit, unsigned char access, unsigned char gran);
void gdt_install();
void toRealMode();
#endif

