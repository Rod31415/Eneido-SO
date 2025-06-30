#include "headers/utilities.h"

char lastkey = ' ';
uint8 shiftOn = 0;

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
void outportl(uint16 port, uint32 data)
{
	__asm__ __volatile__("outl %1, %0" : : "dN"(port), "a"(data));
}

void bflush()
{
	uint8 i = 1;
	while (i++)
		inport(0x60);
}

uint8 keyboard_read()
{

	// while(!(inport(0x64)&1));
	return inport(0x60);
	return 0;
}
char recordLastKey(uint8 input)
{
	return getKeyboardKey(input);
}
uint8 getKeyboardKey(uint8 input)
{
	if (input == 42)
		shiftOn = 1;
	if (input == 170)
		shiftOn = 0;

	switch (input)
	{
  case 27:
    return (shiftOn) ? '*' : '+';
    break;
  case 26:
    return (shiftOn) ? 126 : 94;
    break;
  case 40:
    return (shiftOn) ? '[' : '{';
    break;
  case 43:
    return (shiftOn) ? ']' : '}';
    break;
  case 12:
    return (shiftOn) ? '?' : 39;
    break;
  case 13:
    return (shiftOn) ? '¡' : '¿';
    break;
  case 41:
    return (shiftOn) ? '°' : '|';
    break;
	case 30:
		return (shiftOn) ? 'A' : 'a';
		break;
	case 48:
		return (shiftOn) ? 'B' : 'b';
		break;
	case 46:
		return (shiftOn) ? 'C' : 'c';
		break;
	case 32:
		return (shiftOn) ? 'D' : 'd';
		break;
	case 18:
		return (shiftOn) ? 'E' : 'e';
		break;
	case 33:
		return (shiftOn) ? 'F' : 'f';
		break;
	case 34:
		return (shiftOn) ? 'G' : 'g';
		break;
	case 35:
		return (shiftOn) ? 'H' : 'h';
		break;
	case 23:
		return (shiftOn) ? 'I' : 'i';
		break;
	case 36:
		return (shiftOn) ? 'J' : 'j';
		break;
	case 37:
		return (shiftOn) ? 'K' : 'k';
		break;
	case 38:
		return (shiftOn) ? 'L' : 'l';
		break;
	case 50:
		return (shiftOn) ? 'M' : 'm';
		break;
	case 49:
		return (shiftOn) ? 'N' : 'n';
		break;
	case 39:
		return (shiftOn) ? 'N' : 'n';
		break;
	case 24:
		return (shiftOn) ? 'O' : 'o';
		break;
	case 25:
		return (shiftOn) ? 'P' : 'p';
		break;
	case 16:
		return (shiftOn) ? 'Q' : 'q';
		break;
	case 19:
		return (shiftOn) ? 'R' : 'r';
		break;
	case 31:
		return (shiftOn) ? 'S' : 's';
		break;
	case 20:
		return (shiftOn) ? 'T' : 't';
		break;
	case 22:
		return (shiftOn) ? 'U' : 'u';
		break;
	case 47:
		return (shiftOn) ? 'V' : 'v';
		break;
	case 17:
		return (shiftOn) ? 'W' : 'w';
		break;
	case 45:
		return (shiftOn) ? 'X' : 'x';
		break;
	case 21:
		return (shiftOn) ? 'Y' : 'y';
		break;
	case 44:
		return (shiftOn) ? 'Z' : 'z';
		break;
	case 2:
		return (shiftOn) ? '!' : '1';
		break;
	case 3:
		return (shiftOn) ? '"' : '2';
		break;
	case 4:
		return (shiftOn) ? '#' : '3';
		break;
	case 5:
		return (shiftOn) ? '$' : '4';
		break;
	case 6:
		return (shiftOn) ? '%' : '5';
		break;
	case 7:
		return (shiftOn) ? '&' : '6';
		break;
	case 8:
		return (shiftOn) ? '/' : '7';
		break;
	case 9:
		return (shiftOn) ? '(' : '8';
		break;
	case 10:
		return (shiftOn) ? ')' : '9';
		break;
	case 11:
		return (shiftOn) ? '=' : '0';
		break;
	case 52:
		return (shiftOn) ? ':' : '.';
		break;
	case 51:
		return (shiftOn) ? ';' : ',';
		break;
	case 53:
		return (shiftOn) ? '_' : '-';
		break;
	case 14://TAB
		return 129;
		break;
	case 28:
		return 128;
		break;
	case 80://down
		return 130;
    break;
	case 75://left
		return 131;
    break;
	case 72://up
		return 132;
    break;
	case 77://right
		return 133;
    break;
	case 29:
		return 134;
    break;
  case 1:
		return 135;
    break;

	case 57:
		return ' ';
		break;
	}
	return 0;
}
