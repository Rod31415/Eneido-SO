#include "headers/functions.h"
#include "headers/key_codes.h"
#include "headers/types.h"
#include "headers/kernel.h"
#define colsPerRow 80
#define maxRows 25
int32 globalColumn = 0, globalRow = 0;
char actualColor = 0;

void backspace()
{
	if (globalColumn != 0)
	{
		globalColumn--;
    draw_char(globalColumn*4,globalRow*8,' ',actualColor);
      draw_char(globalColumn*4+4,globalRow*8,' ',actualColor);
	}
}
void scrollDown()
{
scrollDOWN(8,actualColor);
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
    draw_char(globalColumn*4,globalRow*8,48,actualColor);
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
		draw_char(globalColumn*4,globalRow*8,digits[i]+48,actualColor);
		nextPosition();
	}
}

void printHex(uint32 hex)
{

		draw_char(globalColumn*4,globalRow*8,((hex & 0x0f) > 9 ? (hex & 0x0f + 55) : (hex & 0x0f + 48)),actualColor);
	nextPosition();
		draw_char(globalColumn*4,globalRow*8,((hex & 0xf0) / 16 > 9 ? ((hex & 0xf0) / 16 + 55) : ((hex & 0xf0) / 16 + 48)),actualColor);
	nextPosition();
}

void printFlo(float flo)
{
}

void printBin(int32 bin)
{

	if (bin == 0)
	{
    draw_char(globalColumn*4,globalRow*8,48,actualColor);
		nextPosition();
	}
	int32 ref = 0x80;
	while (ref)
	{
		if (bin & ref == ref)
		{
    draw_char(globalColumn*4,globalRow*8,49,actualColor);
			nextPosition();
		}
		else
		{
    draw_char(globalColumn*4,globalRow*8,48,actualColor);
			nextPosition();
		}
		ref >>= 1;
	}
}



void printChr(int32 chr)
{
  draw_char(globalColumn*4,globalRow*8,(uint8)(chr & 255),actualColor);
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
      draw_char(globalColumn*4,globalRow*8,' ',actualColor);
      draw_char(globalColumn*4+4,globalRow*8,' ',actualColor);
			globalColumn = 0;
			globalRow++;
			if (globalRow >= maxRows)
			{
				globalRow=24;
				scrollDown();
			}
			i += 2;
			continue;
		}
  draw_char(globalColumn*4,globalRow*8,(uint8)(str[i] & 255),actualColor);
	nextPosition();
		i++;
	}
}

void cls(uint8 color)
{
  clear_screen(actualColor);

	globalColumn = 0;
	globalRow = 0;
}

void update_cursor(uint32 x, uint32 y)
{
	draw_char(x*4,y*8,255,actualColor);

}

void putChar(int32 x, int32 y, char character, uint8 color)
{
	draw_char(x*4,y*8,character,color);
}
