#ifndef DISKDRIVER_H
#define DISKDRIVER_H
#include "functions.h"
#include "utilities.h"


void initDisk();

void readDiskLBA(uint32 lba,uint8 *buffer);

#endif
