
#ifndef RTL8139_H
#define RTL8139_H

#include "pci.h"
#include "utilities.h"
#include "types.h"
#include "functions.h"
/*
struct regs
{
    unsigned int gs, fs, es, ds;      
    unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;  
    unsigned int int_no, err_code;    
    unsigned int eip, cs, eflags, useresp, ss;  
};*/

struct PCIGeneralDevice;
struct PCIDescriptor;
bool initRtl8139(PCIDescriptor* desc);
void printMAC();
void rtl8139_irq_handler(struct regs *r);
void Rtl8139SendPacket(uint8* MACDest,uint8* buff,uint32 length);
void Rtl8139RecvPacket(uint8* data, uint32 size);
void Rtl8139EnableIRQs();
#endif

