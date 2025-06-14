#include "headers/diskDriver.h"

void comprobeDisk(){
  uint8 flag;
 flag=inport(0x1f7);
if(flag==0)return;

do{
  flag=inport(0x1f7); 
}while(flag&0x80!=0);
     
if(flag&1)return;
    while(!(inport(0x1f7)&8));


}

void initDisk(){


  uint8 flag=inport(0x1f7);
  if(flag==0xFF){printf("No DISK");return;}
  printf("Disk ready --",flag);

  outport(0x1F6,0xA0);

  outport(0x1F2,0x00);
  outport(0x1F3,0x00);
  outport(0x1F4,0x00);
  outport(0x1F5,0x00);

  outport(0x1f7,0xEC);

 
  comprobeDisk();
}

void readDiskLBA(uint32 lba,uint8 *buffer){
uint8 status;

outport(0x1f6,0xE0|((lba>>24)&0x0f));
outport(0x1f2,1);

outport(0x1f3,(uint8)lba);
outport(0x1f4,(uint8)(lba>>8));
outport(0x1f5,(uint8)(lba>>16));


outport(0x1f7,0x20);
comprobeDisk();

for(uint32 i=0;i<256;i++){
  uint16 data=inportw(0x1f0);
  buffer[i*2]=(uint8)data;
  buffer[i*2+1]=(uint8)(data>>8);
}

}

