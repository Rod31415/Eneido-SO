#include "headers/functions.h"
#include "headers/key_codes.h"
#include "headers/types.h"
#include "headers/kernel.h"

#define FontWidth 8
#define FontHeight 16

#define colsPerRow widthWindow / FontWidth
#define maxRows heightWindow / FontHeight

int32 globalColumn = 0, globalRow = 0;
char actualColor = 0;

void backspace()
{
	if (globalColumn != 0)
	{
		globalColumn--;
		draw_char(globalColumn * FontWidth, globalRow * FontHeight, ' ', actualColor);
		draw_char(globalColumn * FontWidth + FontWidth, globalRow * FontHeight, ' ', actualColor);
	}
}
void scrollDown()
{
	scrollDOWN(FontHeight, actualColor);
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

void resetLine()
{
	globalColumn = 0;
}

void printDec(int32 dec, uint8 s = 0)
{
	uint32 digNum = 0;
	uint32 digits[30];
	if (dec == 0)
	{
		draw_char(globalColumn * FontWidth, globalRow * FontHeight, '0', actualColor);
		nextPosition();
		return;
	}
	uint8 sign = 0;
	if (dec < 0 && !s)
	{
		dec = 0U - dec;
		sign = 1;
	}

	while (dec)
	{
		digits[digNum] = (uint32)(dec % 10);
		dec /= 10;
		digNum++;
	}
	if (sign)
	{
		draw_char(globalColumn * FontWidth, globalRow * FontHeight, '-', actualColor);
		nextPosition();
	}
	for (int32 i = digNum - 1; i >= 0; i--)
	{
		draw_char(globalColumn * FontWidth, globalRow * FontHeight, digits[i] + '0', actualColor);
		nextPosition();
	}
}

char nibbleToHexChar(uint8 nibble){
	nibble&=0x0F;
	if(nibble<=9){
		return nibble + '0';
	}
	else{
		return nibble - 10 + 'A';
	}
}

void printHex(uint32 hex, uint8 opt = 0)
{
	if (opt)
	{
		char high_char=nibbleToHexChar((hex&0xF0)>>4);
		char  low_char=nibbleToHexChar(hex&0x0F);
		draw_char(globalColumn * FontWidth, globalRow * FontHeight, high_char, actualColor);
		nextPosition();
		draw_char(globalColumn * FontWidth, globalRow * FontHeight, low_char, actualColor);
		nextPosition();
		return;
	}

	uint32 digNum = 0;
	int8 digits[8];
	while (hex)
	{
		digits[digNum] = ((hex & 0x0f) > 9 ? (hex & 0x0f + 'A' - 10) : ((hex & 0x0f) + '0'));
		hex >>= 4;
		digNum++;
	}
	for (int32 i = digNum - 1; i >= 0; i--)
	{
		draw_char(globalColumn * FontWidth, globalRow * FontHeight, digits[i], actualColor);
		nextPosition();
	}
}

void printFlo(float flo)
{
}

void printBin(int32 bin)
{

	if (bin == 0)
	{
		draw_char(globalColumn * FontWidth, globalRow * FontHeight, 48, actualColor);
		nextPosition();
	}
	int32 ref = 0x80;
	while (ref)
	{
		if (bin & ref == ref)
		{
			draw_char(globalColumn * FontWidth, globalRow * FontHeight, 49, actualColor);
			nextPosition();
		}
		else
		{
			draw_char(globalColumn * FontWidth, globalRow * FontHeight, 48, actualColor);
			nextPosition();
		}
		ref >>= 1;
	}
}

void printChr(uint32 chr)
{
	draw_char(globalColumn * FontWidth, globalRow * FontHeight, (uint8)(chr & 255), actualColor);
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

void printf(char *str, int32 arg0, int32 arg1, int32 arg2, int32 arg3, int32 arg4)
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
				printDec(args[argNum], 0);
				i += 2;
				argNum++;
				continue;
			}
			if (str[i + 1] == 'u')
			{
				printDec(args[argNum], 1);
				i += 2;
				argNum++;
				continue;
			}
			if (str[i + 1] == 'x')
			{
				printHex(args[argNum], 1);
				i += 2;
				argNum++;
				continue;
			}
			if (str[i + 1] == 'h')
			{
				printHex(args[argNum], 0);
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
		else if (str[i] == '/' && str[i + 1] == 'n' || str[i] == 13)
		{
			draw_char(globalColumn * FontWidth, globalRow * FontHeight, ' ', actualColor);
			globalColumn = 0;
			globalRow++;
			if (globalRow >= maxRows)
			{
				globalRow = maxRows - 1;
				scrollDown();
			}
			i += 2;
			continue;
		}
		draw_char(globalColumn * FontWidth, globalRow * FontHeight, (uint8)(str[i] & 255), actualColor);
		nextPosition();
		i++;
	}
}

void getConsoleCursorPosition(uint32 *x, uint32 *y)
{
	*x = globalColumn;
	*y = globalRow;
}

void cls(uint8 color)
{
	clear_screen(actualColor);

	globalColumn = 0;
	globalRow = 0;
}

void update_cursor(uint32 x, uint32 y)
{
	change_ground_color(0);
	draw_char(x * FontWidth, y * FontHeight, '_', actualColor);
	change_ground_color(1);
}

void putChar(int32 x, int32 y, char character, uint8 color)
{
	draw_char(x * FontWidth, y * FontHeight, character, color);
}
