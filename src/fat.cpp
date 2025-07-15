#include "headers/fat.h"


bios_parameter_block_FAT32 localBPB;
FSInfo localFS;
uint32 firstDataSector;
uint32 dataCount;



void initFAT(uint8* buffer)
{
    for(uint32 i=0;i<512;i++)
    *((uint32*)&localBPB+i)=buffer[i];


firstDataSector = localBPB.BPB_RsvdSecCnt + (localBPB.BPB_NumFATs * localBPB.BPB_FATSz32);
dataCount = localBPB.BPB_TotSec32 - (localBPB.BPB_RsvdSecCnt + (localBPB.BPB_NumFATs * localBPB.BPB_FATSz32));
dataCount=dataCount/2;
uint32 FirstRootDirSecNum=localBPB.BPB_RootClus;

printf("FirstDataSector : %u/n",firstDataSector);
printf("Count of cluster : %u%u/n",dataCount/10,dataCount%10);
printf("Entry : %u/n",FirstRootDirSecNum);

//uint8 firstEntry[512];

//readDiskLBA(FirstRootDirSecNum,firstEntry);
}

uint32 sectorByCluster(uint32 cluster)
{

    return ((cluster - 2) * localBPB.BPB_SecPerClus) + firstDataSector;
}
