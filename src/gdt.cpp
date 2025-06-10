#include "headers/gdt.h"

GDT_Entry gdt[3];
struct gdt_ptr gp;
extern "C" uint32 gpt;
extern "C" void gdt_flush(void* gptr);

//extern "C" void switch_to_real_mode(void *gptr);

//extern "C" void readDisk(void);

void gdt_set_gate(int num, unsigned long base, unsigned long limit, unsigned char access, unsigned char gran)
{
    gdt[num].BaseLO = (base & 0xFFFF);
    gdt[num].BaseME = (base >> 16) & 0xFF;
    gdt[num].BaseHI = (base >> 24) & 0xFF;

    gdt[num].LimitLO = (limit & 0xFFFF);
    gdt[num].FlsLimitHI = ((limit >> 16) & 0x0F);

    gdt[num].FlsLimitHI |= (gran & 0xF0);
    gdt[num].AccessByte = access;
}

void gdt_install()
{
    gp.limit = (sizeof(GDT_Entry) * 3) - 1;
    gp.base = (uint32)&gdt;
    gdt_set_gate(0, 0, 0, 0, 0);

    gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);

    gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);
    
    gdt_flush(&gp);
}





