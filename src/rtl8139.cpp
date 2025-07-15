#include "headers/rtl8139.h"

struct RTL8139{
PCIDescriptor* descriptor;
PCIGeneralDevice* device;
uint32 iobase;
uint8 MAC[6];

};
uint32 generalIobase;
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

RTL8139* actualRTL;

PCIDescriptor* d;

uint8 rx_buffer[8192+16];
uint8 tx_buffer[4][1536];

bool isRtl8139(PCIDescriptor *desc){
if(desc->vendor_id==0x10EC&&desc->device_id==0x8139)return true;
return false;
}


void Rtl8139SendPacket(uint8* MACDest,uint8* buff,uint32 length);


void rtl8139_irq_handler(){
  uint16 status=inport(actualRTL->iobase+0x3E);

  if(status&(1<<2)){
    printf("Transmision Completa/n");
    outportw(actualRTL->iobase+0x3E,(1<<2));
  }
  if(status&(1<<0)){
    printf("Paquete Recibido/n");
    outportw(actualRTL->iobase+0x3E,(1<<0));
  }
  if(status&~((1<<2)|(1<<0))){
    printf("Otra interrupcion");  
    outportw(actualRTL->iobase+0x3E,status&~((1<<2)|(1<<0)));
  }
}

bool initRtl8139(PCIDescriptor *desc){
if(!isRtl8139(desc)){
    printf("TODO MAL/n");
    return false;
}


PCIBusMastering(desc);

PCIGeneralDevice *rtl;
PCIGetGeneralDevice(desc,rtl);


actualRTL->device=rtl;
actualRTL->iobase=rtl->base_address[0]&~0x3;


//Prendiendo el rtl
outport(actualRTL->iobase+0x52,0x0);

//Software reset
outport(actualRTL->iobase+0x37,0x10);
while(inport(actualRTL->iobase+0x37)&0x10);


//Iniciar el buffer para recibir datos
outportl(actualRTL->iobase+0x30,(uint32)rx_buffer);

//Set the Interrupt Mask Register (IMR) and Interrupt Service Register (ISR)
//outportw(actualRTL->iobase+0x3c,0x0000);

//outportw(actualRTL->iobase+0x3e,0xFFFF);

//Pa configurar el buffer
outportl(actualRTL->iobase+0x44,(0xf<<7));

//Habilitar RX y TX
outport(actualRTL->iobase+0x37,0x0C);


uint32 m0=inportl(actualRTL->iobase+RTL8139_REG_MAC0_5);
uint32 m1=inportw(actualRTL->iobase+RTL8139_REG_MAC5_6);


  actualRTL->MAC[0]=m0;
  actualRTL->MAC[1]=m0>>8;
  actualRTL->MAC[2]=m0>>16;
  actualRTL->MAC[3]=m0>>24;

  actualRTL->MAC[4]=m1;
  actualRTL->MAC[5]=m1>>8;

  printf("  |%d|  ",actualRTL->iobase);
  generalIobase=actualRTL->iobase;
/*uint8 mac[6]={0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
uint8 buf[]="adios";
    Rtl8139SendPacket(mac,buf,sizeof(buf)-1);
uint8 buf2[]="asdasd";
    Rtl8139SendPacket(mac,buf2,sizeof(buf2)-1);*/



return true;
/*
08:00:27:6a:6b:2e

RTL8139_REG_MAC0_5 = 0x00,
  RTL8139_REG_MAC5_6 = 0x04,
  RTL8139_REG_MAR0_7 = 0x08,
  RTL8139_REG_RBSTART = 0x30,
  RTL8139_REG_CMD = 0x37,
  RTL8139_REG_IMR = 0x3C,
  RTL8139_REG_ISR = 0x3E,

  RTL8139_REG_POWERUP = 0x52*/

}

int32 current_tx=0;

uint32 txAddrRegs[4]={0x20,0x24,0x28,0x2c};
uint32 txStatRegs[4]={0x10,0x14,0x18,0x1c};

void Rtl8139SendPacket(uint8* MACDest,uint8* buff,uint32 length){
  actualRTL->iobase=generalIobase;
  printf("|%d| : %d bytes",actualRTL->iobase,length);
  uint8* newbuffer= tx_buffer[current_tx];
  for(uint32 i=0;i<6;i++){
    newbuffer[i]=MACDest[i];
    newbuffer[i+6]=actualRTL->MAC[i];
  }
  newbuffer[12]=0x08;
  newbuffer[13]=0x00;

  for(uint32 i=0;i<length;i++){
    newbuffer[i+14]=buff[i];
  }
  uint32 total_len=length+14;
  if(total_len<60){
    for(uint32 i=length+14;i<60;i++){
      newbuffer[i]=0;
    }
    total_len=60;
  }

  outportl(actualRTL->iobase+txAddrRegs[current_tx],(uint32)newbuffer);
  outportl(actualRTL->iobase+txStatRegs[current_tx],total_len&0x1FFF);

  
  printf("%d ",current_tx);


  uint32 wait=0;
  while(1){
  uint32 isr = inportw(actualRTL->iobase+0x3e);

if ((isr & (1<<2))) {
    printf("Transmision Completa!/n");
    outportw(actualRTL->iobase+0x3e,(1<<2));
    break;
}
if(++wait>10000000){
  printf("Tiempo agotado/n");
  break;
}

  }
current_tx=(current_tx+1)%4;


}
