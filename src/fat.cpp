#include "headers/fat.h"

typedef struct
{
    uint8 BS_jmpBoot[3];
    uint8 BS_OEMName[8];
    uint16 BPB_BytsPerSec;
    uint8 BPB_SecPerClus;
    uint16 BPB_RsvdSecCnt;
    uint8 BPB_NumFATs;
    uint16 BPB_RootEntCnt;
    uint16 BPB_TotSec16;
    uint8 BPB_Media;
    uint16 BPB_FATSz16;
    uint16 BPB_SecPerTrk;
    uint16 BPB_NumHeads;
    uint32 BPB_HiddSec;
    uint32 BPB_TotSec32;
    uint32 BPB_FATSz32;
    uint16 BPB_ExtFlags;
    uint16 BPB_FSVer;
    uint32 BPB_RootClus;
    uint16 BPB_FSInfo;
    uint16 BPB_BkBootSec;
    uint8 BPB_Reserved[12];
    uint8 BS_DrvNum;
    uint8 BS_Reserved1;
    uint8 BS_BootSig;
    uint32 BS_VolID;
    uint8 BS_VolLab[11];
    uint8 BS_FilSysType[8];
    uint8 zero0[420];
    uint16 Signature_word;

} __attribute__((packed)) bios_parameter_block_FAT32;

typedef struct
{
    uint32 FSI_LeadSig;
    uint8 FSI_Reserved1[480];
    uint32 FSI_StrucSig;
    uint32 FSI_Free_Count;
    uint32 FSI_Nxt_Free;
    uint8 FSI_Reserved2[12];
    uint32 FSI_TrailSig;

} __atribute__((packed)) FSInfo;

typedef struct
{
    uint8 DIR_Name[11];
    uint8 DIR_Attr;
    uint8 DIR_NTRes;
    uint8 DIR_CrtTimeTenth;
    uint16 DIR_CrtTime;
    uint16 DIR_CrtDate;
    uint16 DIR_LstAccDate;
    uint16 DIR_FstClusHI;
    uint16 DIR_WrtTime;
    uint16 DIR_WrtDate;
    uint16 DIR_FstClusLO;
    uint32 DIR_FileSize;

} __atribute__((packed)) DIR;

bios_parameter_block_FAT32 localBPB;
FSInfo localFS;
uint32 firstDataSector;
uint32 dataCount;

void initFATfs(bios_parameter_block_FAT32 bpb)
{

    localBPB = bpb;

    firstDataSector = localBPB->BPB_RsvdSecCnt + (localBPB->BPB_NumFATs * local->BPB_FATSz32);
    dataCount = localBPB->BPB_TotSec32 - (localBPB->BPB_RsvdSecCnt + (localBPB->BPB_NumFATs * localBPB->BPB_FATSz32))
}

uint32 sectorByCluster(uint32 cluster)
{

    return ((cluster - 2) * localBPB->BPB_SecPerClus) + firstDataSector;
}
