
#ifndef RTL8139_H
#define RTL8139_H

#include "pci.h"
#include "utilities.h"
#include "types.h"
#include "functions.h"

struct PCIGeneralDevice;
struct PCIDescriptor;
bool initRtl8139(PCIDescriptor* desc);

#endif

