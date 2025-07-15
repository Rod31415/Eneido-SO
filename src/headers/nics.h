
#ifndef NICS_H
#define NICS_H

#include "pci.h"
#include "types.h"
#include "functions.h"
#include "ne2000.h"
#include "rtl8139.h"

struct PCIDescriptor;
void initNics(PCIDescriptor* desc);

#endif