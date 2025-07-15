
#ifndef NE2000_H
#define NE2000_H

#include "pci.h"
#include "types.h"
#include "functions.h"

struct PCIGeneralDevice;
struct PCIDescriptor;
bool initNe2000(PCIDescriptor* desc);

#endif

