#include "headers/functions.h"
#include "headers/key_codes.h"
#include "headers/types.h"
#include "headers/kernel.h"
#define VGA_MEMORY_ADDRESS 0xb8000
#define colsPerRow 80

int32 globalColumn = 0, globalRow = 0;
char actualColor = 0;

void backspace()
{
	if (globalColumn != 0)
	{
		globalColumn--;
		*(char *)(VGA_MEMORY_ADDRESS + (globalColumn + globalRow * colsPerRow) * 2) = ' ';
		*(char *)(VGA_MEMORY_ADDRESS + (globalColumn + globalRow * colsPerRow) * 2 + 1) = actualColor;
	}
}
void scrollDown()
{

	memcpy(VGA_MEMORY_ADDRESS + 160, VGA_MEMORY_ADDRESS, 3840);
	for (uint8 i = 0; i < 160; i += 2)
	{
		*(char *)(VGA_MEMORY_ADDRESS + 3840 + i) = ' ';
		*(char *)(VGA_MEMORY_ADDRESS + 3840 + i + 1) = actualColor;
	}
}

void sleep()
{
	for (int i = 0; i < 1000000; i++)
	{
		__asm__ __volatile__("nop");
	}
}

void nextPosition()
{
	globalColumn++;
	if (globalColumn == colsPerRow)
	{
		globalColumn = 0;
		globalRow++;
	}
};

void resetLine(){
globalColumn=0;

}

void printDec(int32 dec)
{
	int32 digNum = 0;
	int32 digits[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	if (dec == 0)
	{
		*(char *)(VGA_MEMORY_ADDRESS + (globalColumn + globalRow * colsPerRow) * 2) = 48;
		*(char *)(VGA_MEMORY_ADDRESS + (globalColumn + globalRow * colsPerRow) * 2 + 1) = actualColor;
		nextPosition();
		return;
	}

	while (dec)
	{
		digits[digNum] = dec % 10;
		dec /= 10;
		digNum++;
	}

	for (int32 i = digNum - 1; i >= 0; i--)
	{
		*(char *)(VGA_MEMORY_ADDRESS + (globalColumn + globalRow * colsPerRow) * 2) = digits[i] + 48;
		*(char *)(VGA_MEMORY_ADDRESS + (globalColumn + globalRow * colsPerRow) * 2 + 1) = actualColor;
		nextPosition();
	}
}

void printHex(uint32 hex)
{

	*(char *)(VGA_MEMORY_ADDRESS + (globalColumn + globalRow * colsPerRow) * 2) = ((hex & 0x0f) > 9 ? (hex & 0x0f + 55) : (hex & 0x0f + 48));
	*(char *)(VGA_MEMORY_ADDRESS + (globalColumn + globalRow * colsPerRow) * 2 + 1) = actualColor;
	nextPosition();
	*(char *)(VGA_MEMORY_ADDRESS + (globalColumn + globalRow * colsPerRow) * 2) = ((hex & 0xf0) / 16 > 9 ? ((hex & 0xf0) / 16 + 55) : ((hex & 0xf0) / 16 + 48));
	*(char *)(VGA_MEMORY_ADDRESS + (globalColumn + globalRow * colsPerRow) * 2 + 1) = actualColor;
	nextPosition();
}

void printFlo(float flo)
{
}

void printBin(int32 bin)
{

	if (bin == 0)
	{
		*(char *)(VGA_MEMORY_ADDRESS + (globalColumn + globalRow * colsPerRow) * 2) = 48;
		*(char *)(VGA_MEMORY_ADDRESS + (globalColumn + globalRow * colsPerRow) * 2 + 1) = actualColor;
		nextPosition();
	}
	int32 ref = 0x80;
	while (ref)
	{
		if (bin & ref == ref)
		{
			*(char *)(VGA_MEMORY_ADDRESS + (globalColumn + globalRow * colsPerRow) * 2) = 49;
			*(char *)(VGA_MEMORY_ADDRESS + (globalColumn + globalRow * colsPerRow) * 2 + 1) = actualColor;
			nextPosition();
		}
		else
		{
			*(char *)(VGA_MEMORY_ADDRESS + (globalColumn + globalRow * colsPerRow) * 2) = 48;
			*(char *)(VGA_MEMORY_ADDRESS + (globalColumn + globalRow * colsPerRow) * 2 + 1) = actualColor;
			nextPosition();
		}
		ref >>= 1;
	}
}



void printChr(int32 chr)
{
	*(char *)(VGA_MEMORY_ADDRESS + (globalColumn + globalRow * colsPerRow) * 2) = (uint8)(chr & 255);
	*(char *)(VGA_MEMORY_ADDRESS + (globalColumn + globalRow * colsPerRow) * 2 + 1) = actualColor;
	nextPosition();
}

void changeColor(uint8 color)
{
	actualColor = color;
}

void gotoxy(int32 x, int32 y)
{
	globalColumn = x;
	globalRow = y;
}

void printf(const char *str, int32 arg0, int32 arg1, int32 arg2, int32 arg3, int32 arg4)
{
	int32 i = 0;

	int32 args[5];
	args[0] = arg0;
	args[1] = arg1;
	args[2] = arg2;
	args[3] = arg3;
	int32 argNum = 0;
	while (str[i] != 0)
	{
		if (str[i] == '%')
		{
			if (str[i + 1] == 'd')
			{
				printDec(args[argNum]);
				i += 2;
				argNum++;
				continue;
			}
			if (str[i + 1] == 'x')
			{
				printHex(args[argNum]);
				i += 2;
				argNum++;
				continue;
			}
			if (str[i + 1] == 'f')
			{
				printFlo(args[argNum]);
				i += 2;
				argNum++;
				continue;
			}
			if (str[i + 1] == 'b')
			{
				printBin(args[argNum]);
				i += 2;
				argNum++;
				continue;
			}
			if (str[i + 1] == 'c')
			{
				printChr(args[argNum]);
				i += 2;
				argNum++;
				continue;
			}
		}
		else if (str[i] == '/' && str[i + 1] == 'n')
		{
			globalColumn = 0;
			globalRow++;
			if (globalRow > 24)
			{
				globalRow--;
				scrollDown();
			}
			if (str[i + 3] == 'n')
			{
				*(char *)(VGA_MEMORY_ADDRESS + (globalColumn + globalRow * colsPerRow) * 2) = str[i + 2];
				*(char *)(VGA_MEMORY_ADDRESS + (globalColumn + globalRow * colsPerRow) * 2 + 1) = actualColor;
				nextPosition();
				i += 2;
			}

			i += 2;
			continue;
		}
		*(char *)(VGA_MEMORY_ADDRESS + (globalColumn + globalRow * colsPerRow) * 2) = str[i];
		*(char *)(VGA_MEMORY_ADDRESS + (globalColumn + globalRow * colsPerRow) * 2 + 1) = actualColor;
		i++;
		nextPosition();
	}
}

void cls(uint8 color)
{
	for (int32 j = 0; j < 25; j++)
	{
		for (int32 i = 0; i < colsPerRow; i++)
		{
			*(char *)(VGA_MEMORY_ADDRESS + (i + j * colsPerRow) * 2) = ' ';
			*(char *)(VGA_MEMORY_ADDRESS + (i + j * colsPerRow) * 2 + 1) = color;
		}
	}

	globalColumn = 0;
	globalRow = 0;
}

void update_cursor(uint32 x, uint32 y)
{
	uint16 pos = y * colsPerRow + x;

	outport(0x3D4, 0x0F);
	outport(0x3D5, (uint8)(pos & 0xFF));
	outport(0x3D4, 0x0E);
	outport(0x3D5, (uint8)((pos >> 8) & 0xFF));
}

void putChar(int32 x, int32 y, char character, uint8 color)
{
	*(char *)(VGA_MEMORY_ADDRESS + (x + y * colsPerRow) * 2) = character;
	*(char *)(VGA_MEMORY_ADDRESS + (globalColumn + globalRow * colsPerRow) * 2 + 1) = color;
}
