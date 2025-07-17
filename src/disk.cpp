#include "headers/diskDriver.h"

void checkDisk()
{
	uint8 flag;
	flag = inport(0x1f7);
	if (flag != 0 && flag != 0xff)
	{
		printf("Ata DRIVE found! status : %d", flag);
	}
	else
	{
		printf("Ata drive not found!");
	}
}

void resetDisk()
{
	outport(0x3f6, 6);
	outport(0x3f6, 2);
}

uint8 isBusyDisk()
{
	uint8 sta = inport(0x1f7);
	return (sta & 0x80) != 0;
}
uint8 isReadyDisk()
{
	uint8 sta = inport(0x1f7);
	return (sta & 0x40) != 0;
}

void initDisk()
{

	checkDisk();
}

void readDiskLBA(uint32 lba, uint8 *buffer)
{

	while (isBusyDisk())
		;

	outport(0x1f6, 0xE0 | ((lba >> 24) & 0x0f));
	outport(0x1f1, 0);
	outport(0x1f2, 1);

	outport(0x1f3, (uint8)lba);
	outport(0x1f4, (uint8)(lba >> 8));
	outport(0x1f5, (uint8)(lba >> 16));

	outport(0x1f7, 0x20);
	// outport(0x3f6,2);

	while (isBusyDisk())
		;

	for (uint32 i = 0; i < 256; i++)
	{
		uint16 data = inportw(0x1f0);
		buffer[i * 2] = (uint8)data;
		buffer[i * 2 + 1] = (uint8)(data >> 8);
	}
	resetDisk();
}
void writeDiskLBA(uint32 lba, uint8 *buffer)
{

	while (isBusyDisk())
		;

	outport(0x1f6, 0xE0 | ((lba >> 24) & 0x0f));
	outport(0x1f1, 0);
	outport(0x1f2, 1);

	outport(0x1f3, (uint8)lba);
	outport(0x1f4, (uint8)(lba >> 8));
	outport(0x1f5, (uint8)(lba >> 16));

	outport(0x1f7, 0x30);
	// outport(0x3f6,2);

	while (isBusyDisk())
		;

	for (uint32 i = 0; i < 256; i++)
	{
		outportw(0x1f0, ((uint16 *)buffer)[i]);
	}
	resetDisk();
}
