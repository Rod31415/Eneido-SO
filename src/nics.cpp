
#include "headers/nics.h"

void initNics(PCIDescriptor *desc)
{
    if (initRtl8139(desc))
    {
	printf("NIC Correct!");
    }
}