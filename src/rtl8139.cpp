#include "headers/rtl8139.h"

enum RTL8139Registers {
  RTL8139_REG_MAC0_5 = 0x00,
  RTL8139_REG_MAC5_6 = 0x04,
  RTL8139_REG_MAR0_7 = 0x08,
  RTL8139_REG_RBSTART = 0x30,
  RTL8139_REG_CMD = 0x37,
  RTL8139_REG_IMR = 0x3C,
  RTL8139_REG_ISR = 0x3E,

  RTL8139_REG_POWERUP = 0x52
};

uint8 rx_buffer[8192+16];

bool isRtl8139(PCIDescriptor *desc){
if(desc->vendor_id==0x10EC&&desc->device_id==0x8139)return true;
return false;
}

bool initRtl8139(PCIDescriptor *desc){
if(!isRtl8139(desc)){
    printf("TODO MAL/n");
    return false;
}

PCIBusMastering(desc);

PCIGeneralDevice *rtl;

PCIGetGeneralDevice(desc,rtl);

uint32 iobase=rtl->base_address[0]&~0x3;

//Prendiendo el rtl
outport(iobase+0x52,0x0);

//Software reset
outport(iobase+0x37,0x10);

while((inport(iobase+0x37)&0x10)!=0){}

//Iniciar el buffer para recibir datos
outportl(iobase+0x30,(uint32)rx_buffer);

//Set the Interrupt Mask Register (IMR) and Interrupt Service Register (ISR)
outportw(iobase+0x3c,0x0005);


//Pa configurar el buffer
outportl(iobase+0x44,0xf|(1<<7));

//Habilitar RX y TX
outport(iobase+0x37,0x0C);


uint32 m0=inportl(iobase+RTL8139_REG_MAC0_5);
uint32 m1=inportw(iobase+RTL8139_REG_MAC5_6);

printf("MAC ADDRESS: %x:%x:%x:%x",m0,m0>>8,m0>>16,m0>>24);
printf(":%x:%x /n",m1,m1>>8);

printf("TODO BIEN/n");

return true;

/*
RTL8139_REG_MAC0_5 = 0x00,
  RTL8139_REG_MAC5_6 = 0x04,
  RTL8139_REG_MAR0_7 = 0x08,
  RTL8139_REG_RBSTART = 0x30,
  RTL8139_REG_CMD = 0x37,
  RTL8139_REG_IMR = 0x3C,
  RTL8139_REG_ISR = 0x3E,

  RTL8139_REG_POWERUP = 0x52*/

}