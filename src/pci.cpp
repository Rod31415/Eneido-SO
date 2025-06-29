#include "headers/pci.h"

#define CONFIG_ADDRESS 0xCF8
#define CONFIG_DATA 0xCFC

struct PCIDescriptor{
uint32 portBase;
uint32 interrupt;

uint16 bus;
uint16 device;
uint16 function;

uint16 vendor_id;
uint16 device_id;

uint8 class_id;
uint8 subclass_id;
uint8 interface_id;

uint8 revision;

};

  
uint32 PCIReadWord(uint8 bus, uint8 device, uint8 func, uint8 offset) {
    uint32 address;
    uint32 lbus  = (uint32)bus;
    uint32 lslot = (uint32)device;
    uint32 lfunc = (uint32)func;
    uint32 tmp = 0;
  
    // Create configuration address as per Figure 1
    address = (uint32)((lbus << 16) | (lslot << 11) |
              (lfunc << 8) | (offset & 0xFC) | (0x1<<31));
  
    // Write out the address
    outportl(CONFIG_ADDRESS, address);
    // (offset & 2) * 8) = 0 will choose the first word of the 32-bit register
    tmp = inportl(CONFIG_DATA);
    return tmp>>(8*(offset%4));
}

void PCIWriteWord(uint8 bus, uint8 device, uint8 func, uint8 offset,uint32 value) {
    uint32 address;
    uint32 lbus  = (uint32)bus;
    uint32 lslot = (uint32)device;
    uint32 lfunc = (uint32)func;
    uint16 tmp = 0;
  
    // Create configuration address as per Figure 1
    address = (uint32)((lbus << 16) | (lslot << 11) |
              (lfunc << 8) | (offset & 0xFC) | (0x1<<31));
  
    // Write out the address
    outportl(CONFIG_ADDRESS, address);
    // (offset & 2) * 8) = 0 will choose the first word of the 32-bit register
}

uint16 PCICheckVendor(uint8 bus, uint8 device) {

  return PCIReadWord(bus,device,0,0x0E)&(1<<7);

}

PCIDescriptor PCIGetDescriptor(uint16 bus,uint16 device,uint16 function){
struct PCIDescriptor result;

result.bus=bus;
result.device=device;
result.function=function;

result.vendor_id=PCIReadWord(bus,device,function,0x00);
result.device_id=PCIReadWord(bus,device,function,0x02);

result.class_id=PCIReadWord(bus,device,function,0x0B);
result.subclass_id=PCIReadWord(bus,device,function,0x0A);
result.interface_id=PCIReadWord(bus,device,function,0x09);

result.revision=PCIReadWord(bus,device,function,0x08);
result.interrupt=PCIReadWord(bus,device,function,0x3c);


return result;
}

void PCIInitDrivers(){

  for(uint32 bus=0;bus<8;bus++){
    for(uint32 device=0;device<32;device++){
      uint32 maxFunctions=PCICheckVendor(bus,device)?8:1;
      for(uint32 functions=0;functions<maxFunctions;functions++){
        PCIDescriptor dev=PCIGetDescriptor(bus,device,functions);

        if(dev.vendor_id==0x0000||dev.vendor_id==0xFFFF)
          break;
          
      }
    }
  }

}

int8 *PCIDeviceClasses[20]={
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


void PCIShowDevices(){

  for(uint32 bus=0;bus<8;bus++){
    for(uint32 device=0;device<32;device++){
      uint32 maxFunctions=PCICheckVendor(bus,device)?8:1;
      for(uint32 functions=0;functions<maxFunctions;functions++){
        PCIDescriptor dev=PCIGetDescriptor(bus,device,functions);

        if(dev.vendor_id==0x0000||dev.vendor_id==0xFFFF)
          break;

        printf("BUS %d, DEV %d, FN %d ",bus,device,functions);
        printf("= VEN %d, DEV %d : ",dev.vendor_id,dev.device_id);
        if(dev.class_id<=20){
        printf(PCIDeviceClasses[dev.class_id]);
        }
        printf("/n");
      }
    }
  }

}

