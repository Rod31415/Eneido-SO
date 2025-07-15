
#include "headers/ne2000.h"


bool isNe2000(PCIDescriptor *device) {
  return (device->vendor_id == 0x10ec && device->device_id == 0x8029);
}

bool initNe2000(PCIDescriptor* desc){
if(!isNe2000(desc))return false;


PCIGeneralDevice *neDevice;
PCIGetGeneralDevice(desc,neDevice);

uint32 iobase=neDevice->base_address[0]&~0x3;

outport(iobase+0x1f,inport(iobase+0x1f));

while((inport(iobase+0x07)&0x80)==0);
outport(iobase+0x07,0xFF);

uint8 prom[32];

outport(iobase,(1<<5)|1);
outport(iobase + 0x0e,0x49);
outport(iobase + 0x0A, 0);		// clear the count regs
outport(iobase + 0x0B, 0);
outport(iobase + 0x0F, 0);		// mask completion IRQ
outport(iobase + 0x07, 0xFF);
outport(iobase + 0x0C, 0x20);		// set to monitor
outport(iobase + 0x0D, 0x02);		// and loopback mode.
outport(iobase + 0x0A, 32);		// reading 32 bytes
outport(iobase + 0x0B, 0);		// count high
outport(iobase + 0x08, 0);		// start DMA at 0
outport(iobase + 0x09, 0);		// start DMA high
outport(iobase, 0x0A);	

for(uint32 i=0;i<32;i++){
    prom[i]=inport(iobase+0x10);
printf("%h ",prom[i]);
}

return true;


}