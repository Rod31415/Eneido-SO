#include "headers/utilities.h"


void memcpy(uint32 origin, uint32 destiny, uint32 size)
{

	for (uint32 i = 0; i < size; i++)
	{
		*(uint8 *)(destiny + i) = *(uint8 *)(origin + i);
	}
}
void memset(uint32 address, uint8 num, uint32 size)
{

	for (uint32 i = 0; i < size; i++)
	{
		*(uint8 *)(address + i) = num;
	}
}

uint8 inport(uint16 port)
{
	uint8 rv;
	__asm__ __volatile__("inb %1, %0" : "=a"(rv) : "dN"(port));
	return rv;
}
uint16 inportw(uint16 port)
{
	uint16 rv;
	__asm__ __volatile__("inw %1, %0" : "=a"(rv) : "dN"(port));
	return rv;
}
uint32 inportl(uint16 port)
{
	uint32 rv;
	__asm__ __volatile__("inl %1, %0" : "=a"(rv) : "dN"(port));
	return rv;
}

void outport(uint16 port, uint8 data)
{
	__asm__ __volatile__("outb %1, %0" : : "dN"(port), "a"(data));
}

void outportw(uint16 port, uint16 data)
{
	__asm__ __volatile__("outw %1, %0" : : "dN"(port), "a"(data));
}

void outportl(uint16 port, uint32 data)
{
	__asm__ __volatile__("outl %1, %0" : : "dN"(port), "a"(data));
}


