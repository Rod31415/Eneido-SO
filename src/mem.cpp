#include "headers/mem.h"
void showMem(uint32 origin, uint32 destiny){

for(uint32 i=origin;i<destiny;i+=16){
    printf("/n%d : ",i);
for(uint32 j=0;j<16;j++)
    printf("%d ",*(uint8*)(i+j));

  }


}
