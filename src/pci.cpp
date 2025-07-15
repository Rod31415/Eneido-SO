
#include "headers/pci.h"

#define CONFIG_ADDRESS 0xCF8
#define CONFIG_DATA 0xCFC




  
uint32 PCIReadWord(uint8 bus, uint8 device, uint8 func, uint8 offset) {
    uint32 address;
    uint32 lbus  = (uint32)bus;
    uint32 lslot = (uint32)device;
    uint32 lfunc = (uint32)func;
    uint32 tmp = 0;
  
    address = (uint32)((lbus << 16) | (lslot << 11) |
              (lfunc << 8) | (offset & 0xFC) | (0x1<<31));
  
    outportl(CONFIG_ADDRESS, address);
    //tmp = (uint16)((inportl(CONFIG_DATA) >> ((offset & 2) * 8)) & 0xFFFF);
    tmp=inportl(CONFIG_DATA);

    
    return tmp>>(8*(offset%4));
}

void PCIWriteWord(uint8 bus, uint8 device, uint8 func, uint8 offset,uint32 value) {
    uint32 address;
    uint32 lbus  = (uint32)bus;
    uint32 lslot = (uint32)device;
    uint32 lfunc = (uint32)func;
  
    address = (uint32)((lbus << 16) | (lslot << 11) |
              (lfunc << 8) | (offset & 0xFC) | (0x1<<31));
  
    outportl(CONFIG_ADDRESS, address);
    outportl(CONFIG_DATA,value);
  }

uint16 PCICheckVendor(uint8 bus, uint8 device) {

  return PCIReadWord(bus,device,0,0x0E)&(1<<7);

}

void PCIGetDescriptor(PCIDescriptor *result,uint16 bus,uint16 device,uint16 function){

result->bus=bus;
result->device=device;
result->function=function;

result->vendor_id=PCIReadWord(bus,device,function,PCI_VENDOR_ID);
result->device_id=PCIReadWord(bus,device,function,PCI_DEVICE_ID);

result->class_id=PCIReadWord(bus,device,function,PCI_CLASS_CODE);
result->subclass_id=PCIReadWord(bus,device,function,PCI_SUBCLASS_CODE);
result->interface_id=PCIReadWord(bus,device,function,PCI_INTERFACE_ID);

result->revision=PCIReadWord(bus,device,function,PCI_REVISION_ID);
result->interrupt=PCIReadWord(bus,device,function,PCI_INTERRUPT_LINE);
result->header_type=PCIReadWord(bus,device,function,PCI_HEADER_TYPE);

result->command=PCIReadWord(bus,device,function,PCI_COMMAND);
result->status=PCIReadWord(bus,device,function,PCI_STATUS);
}

void PCIGetGeneralDevice(PCIDescriptor *desc, PCIGeneralDevice *dev){
  uint32 s=0;
  for(s=0;s<6;s++){
  if(s>=1)break;
  //dev->base_address[s]=COMBINE_WORD(PCIReadWord(desc->bus,desc->device,desc->function,PCI_BASE_ADDR0+s*4+2),PCIReadWord(desc->bus,desc->device,desc->function,PCI_BASE_ADDR0+s*4));//((((uint32)PCIReadWord(desc->bus,desc->device,desc->function,PCI_BASE_ADDR0+s*4+2))<<16)|PCIReadWord(desc->bus,desc->device,desc->function,PCI_BASE_ADDR0+s*4  ));
  uint32 msb=PCIReadWord(desc->bus,desc->device,desc->function,PCI_BASE_ADDR0+s*4+2);
  uint32 lsb=PCIReadWord(desc->bus,desc->device,desc->function,PCI_BASE_ADDR0+s*4);
  dev->base_address[s]=(uint32)((msb<<16)|lsb);

  //printf("%d ",s);
  }
  
}


int8* PCIDeviceClasses[20]={
  "Unclassified Device",
  "Mass Storage Controller",
  "Network Controller",
  "Display Controller",
  "Multimedia Controller",
  "Memory Controller",
  "Bridge",
  "Communication Controller",
  "Generic System Controller",
  "Input Device Controller",
  "Docking Station",
  "Processor",
  "Serial Bus Controller",
  "Wireless Controller",
  "Intelligent Controller",
  "Satellite Communications Controller",
  "Encryption Controller",
  "Signal Processing Controller",
  "Processing Accelerators",
  "Non-Essential Instrumentation"
};

void PCIBusMastering(PCIDescriptor* desc){
desc->command=PCIReadWord(desc->bus,desc->device,desc->function,PCI_COMMAND);
desc->command|=(1<<2);
PCIWriteWord(desc->bus,desc->device,desc->function,PCI_COMMAND,COMBINE_WORD(desc->status,desc->command));
}

void PCIInitDrivers(){
  
for(uint32 bus=0;bus<8;bus++){
    for(uint32 device=0;device<32;device++){
      uint32 maxFunctions=PCICheckVendor(bus,device)?8:1;
      for(uint32 functions=0;functions<maxFunctions;functions++){
        PCIDescriptor dev;
        PCIGetDescriptor(&dev,bus,device,functions);

        if(dev.vendor_id==0x0000||dev.vendor_id==0xFFFF)
          break;
        if((dev.header_type&0x7f)!=0)
          continue;

        switch(dev.class_id){
          case Network_Controller:
          initNics(&dev);
            //return;
          break;
        }

      }
    }
  }

}


void PCIShowDevices(){

  for(uint32 bus=0;bus<8;bus++){
    for(uint32 device=0;device<32;device++){
  
      uint32 maxFunctions=PCICheckVendor(bus,device)?8:1;
         
      for(uint32 functions=0;functions<maxFunctions;functions++){
        PCIDescriptor *dev;
        PCIGetDescriptor(dev,bus,device,functions);

        if(dev->vendor_id==0x0000||dev->vendor_id==0xFFFF)
          break;

        printf("BUS %x, DEV %x, FN %x ",bus,device,functions);
        printf("= VEN %h, DEV %h, HEAD %d: ",dev->vendor_id,dev->device_id,dev->header_type);
        if(dev->class_id<=20){
        printf(PCIDeviceClasses[dev->class_id]);
        }
        printf("/n");
      }
    }
  }
}

