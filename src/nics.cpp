
#include "headers/nics.h"


void initNics(PCIDescriptor* desc){
    if(initNe2000(desc)||initRtl8139(desc)){
        printf("NIC Correct!");
    }
    
}