#ifndef PCI_H
#define PCI_H

#include "utilities.h"
#include "types.h"
#include "functions.h"
#include "nics.h"



#define PCI_VENDOR_ID   0x00
#define PCI_DEVICE_ID   0x02

#define PCI_COMMAND     0x04
#define PCI_STATUS      0x06

#define PCI_REVISION_ID 0x08
#define PCI_INTERFACE_ID 0x09
#define PCI_SUBCLASS_CODE 0x0A
#define PCI_CLASS_CODE  0x0B

#define PCI_CACHE_LINE  0x0c
#define PCI_LAT_TIMER   0x0d
#define PCI_HEADER_TYPE 0x0e
#define PCI_BIST        0x0f

#define PCI_BASE_ADDR0 0x10
#define PCI_BASE_ADDR1 0x14
#define PCI_BASE_ADDR2 0x18
#define PCI_BASE_ADDR3 0x1c
#define PCI_BASE_ADDR4 0x20
#define PCI_BASE_ADDR5 0x24

#define PCI_CARD_BUS 0x28

#define PCI_SUBSYSTEM_VENDOR 0x2c
#define PCI_SUBSYSTEM_ID 0x2e

#define PCI_EXPANSION_ADDR 0x30

#define PCI_CAP_POINTER 0x34

#define PCI_INTERRUPT_LINE 0x3c
#define PCI_INTERRUPT_PIN  0x3d
#define PCI_MIN_GRANT      0x3e
#define PCI_MAX_LATENCY    0x3f

#define COMBINE_WORD(msb,lsb) (((uint32)(msb)<<16)|lsb)



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
uint8 header_type;

uint16 command;
uint16 status;

};

struct PCIGeneralDevice{

uint32 base_address[6];

uint32 card_bus;

uint16 subclass_vendor_id;
uint16 subsystem_id;

uint32 expansion_addres;

uint8 cap_pointer;


uint8 interrupt_line;
uint8 interrupt_pin;
uint8 min_grant;
uint8 max_latency;

};

enum EnumPCIDeviceClasses{
  Unclassified_Device=0,
  Mass_Storage_Controller,
  Network_Controller,
  Display_Controller,
  Multimedia_Controller,
  Memory_Controller,
  Bridge,
  Communication_Controller,
  Generic_System_Controller,
  Input_Device_Controller,
  Docking_Station,
  Processor,
  Serial_Bus_Controller,
  Wireless_Controller,
  Intelligent_Controller,
  Satellite_Communications_Controller,
  Encryption_Controller,
  Signal_Processing_Controller,
  Processing_Accelerators,
  Non_Essential_Instrumentation
};

uint32 PCIReadWord(uint8 bus, uint8 device, uint8 func, uint8 offset);
void PCIWriteWord(uint8 bus, uint8 device, uint8 func, uint8 offset,uint32 value);

void PCIGetDescriptor(PCIDescriptor *result,uint16 bus,uint16 device,uint16 function);
void PCIGetGeneralDevice(PCIDescriptor *desc,PCIGeneralDevice* dev);

void PCIBusMastering(PCIDescriptor *desc);

void PCIShowDevices();

void PCIInitDrivers();

#endif
