#include "headers/editor.h"

uint32 col = 0, row = 0;

uint32 maxRowsInFile = 0;
#define colOffSet 2
#define rowOffSet 0

#define down 130
#define left 131
#define up 132
#define right 133
#define esc 135
#define del 129
#define enter 128

#define height 29
#define width 80

char lineStr[1000][width];

bool kbhit()
{

	uint8 status = inport(0x64);

	return (status & 1);
}
uint8 letter;

uint32 localX = 0, localY = 0;
FILE* localFile;

void drawSide()
{
	draw_rect(0, 0, 16, height * 16, COLOR_LIGHT_GRAY);
	changeColor(COLOR_BLACK);
	change_ground_color(false);
	for (uint32 i = 0; i <= height - 1; i++)
	{
		draw_rect(0, i * 16, 16, 16, COLOR_LIGHT_GRAY);
		gotoxy((i < 10) ? 1 : 0, i);
		printf("%d", i);
	}
	changeColor(COLOR_WHITE);
	change_ground_color(1);
}
void drawBottom()
{
	draw_rect(0, (height - 1) * 16 + 8, width * 8, 24, COLOR_WHITE);
	change_ground_color(false);
	changeColor(COLOR_BLACK);
	gotoxy(1, 29);
	printf("FILE:");
	printf(localFile->Name);
	gotoxy(36, 29);
	printf("EDITOR");
	gotoxy(62, 29);
	printf("ESC for save");
	change_ground_color(1);
	refresh();
	gotoxy(2, 0);
	changeColor(COLOR_WHITE);
}
char buff[5 * 512 * 8];
void initEditor(char* filename)
{

	col = 0;
	row = 0;

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			lineStr[i][j] = 0;
		}
	}

	localFile=fopen(filename);
	cls(COLOR_BLACK);
	
	fgets(buff,sizeof(buff),localFile);
	uint32 rowIndex = 0, colIndex = 0;

	for (uint32 u = 0; u < localFile->FileSize * 512*8; u++)
	{

		if (buff[u] == 13)
		{
			lineStr[rowIndex][colIndex] = 1;
			colIndex = 0;
			rowIndex++;
			u++;
		}
		if(buff[u]==0)break;
		lineStr[rowIndex][colIndex] = (char)buff[u];
		colIndex++;
	}
	changeColor(COLOR_WHITE);
	for (uint32 i = 0; i < height; i++)
	{
		gotoxy(colOffSet, i);
		uint32 index = 0;
		while (lineStr[i][index] != 0)
		{
			printChr(lineStr[i][index]);
			index++;
		}
	}

	drawSide();
	drawBottom();
	// update_cursor(0,0);

	letter = enter;
	loopEditor();
}

void printLine()
{

	uint8 buffer[81 - colOffSet];
	memset((uint32)buffer, 32, 81 - colOffSet - 1);
	buffer[81 - colOffSet] = 0;
	gotoxy(colOffSet, row);
	printf((int8 *)buffer);

	gotoxy(colOffSet, row);
	uint32 index = 0;
	while (lineStr[row][index] != 0)
	{
		printChr(lineStr[row][index]);
		index++;
	}
}

void checkSize()
{
	uint32 size = 0;
	for (uint32 h = 0; h < 1000; h++)
	{
		if (lineStr[h][0] == 0)
		{
			size = h * 80;
			break;
		}
	}
	//localFile.size = (uint32)(size / 512) + 1;
	//modifyFile(localFile.name, localFile);
}

void refreshMaxFileRows()
{
	if (row > maxRowsInFile)
		maxRowsInFile = row;
}

void passRow(uint32 r)
{

	for (uint32 i = height; i > r + 1; i--)
	{
		memcpy((uint32)lineStr[i - 1], (uint32)lineStr[i], 80);
	}

	memset((uint32)lineStr[r + 1], 0, 80);
	// lineStr[r+1][0]=' ';
	splitStr(lineStr[r], lineStr[r + 1], col);
	int8 zerob[40];
	memset((uint32)zerob, 32, 40);
	for (uint32 i = 0; i < height - 1; i++)
	{
		gotoxy(colOffSet, i);
		printf(zerob);
		gotoxy(colOffSet, i);
		uint32 index = 0;
		while (lineStr[i][index] != 0)
		{
			printChr(lineStr[i][index]);
			index++;
		}
	}
}

void quitRow(uint32 r)
{
	int8 buff[80];
	col = lenghtStr(lineStr[r - 1]);
	strcpy(buff, lineStr[r]);
	for (uint32 i = r + 1; i < height; i++)
	{
		memcpy((uint32)lineStr[i], (uint32)lineStr[i - 1], 40);
	}
	appendStr(lineStr[r - 1], buff);

	int8 zerob[79];
	memset((uint32)zerob, 32, 79);
	for (uint32 i = 0; i < height - 1; i++)
	{
		gotoxy(colOffSet, i);
		printf(zerob);
		gotoxy(colOffSet, i);
		uint32 index = 0;
		while (lineStr[i][index] != 0)
		{
			printChr(lineStr[i][index]);
			index++;
		}
	}
	drawSide();
	drawBottom();
}

void loopEditor()
{
	update_cursor(col + colOffSet, row);
	eatKeyBuffered();
	while (1)
	{
		letter = getLastAsciiKey();
		if (!isKeyBuffered())
			continue;
		eatKeyBuffered();

		if (letter == esc)
		{

			uint32 rowIndex=0,colIndex=0;
			uint32 u;
			for (u = 0; u < localFile->FileSize * 512*8; u++)
			{

	if (lineStr[rowIndex][colIndex] == 0)
	{
		if (rowIndex == maxRowsInFile|| lineStr[0][colIndex+1] == 0)
		{
			break;
		}
		else
		{
			rowIndex++;
			colIndex = 0;
			buff[u] = 13;
			u++;
		}
	}
	buff[u] = (uint8)lineStr[rowIndex][colIndex];
	colIndex++;
			}
			buff[u+1]=0;
			fputs(buff,localFile);
			break;
		}
		uint32 lgtStr = lenghtStr(lineStr[row]);
		if (letter == left && col > 0)
		{
			col--;
			printLine();
			update_cursor(col + colOffSet, row);
		}
		else if (letter == right && col < lgtStr)
		{
			col++;
			printLine();
			update_cursor(col + colOffSet, row);
		}
		else if (letter == down && row < height)
		{
			printLine();
			row++;
			col = (col > lenghtStr(lineStr[row])) ? lenghtStr(lineStr[row]) : col;
			printLine();
			update_cursor(col + colOffSet, row);
			refreshMaxFileRows();
		}
		else if (letter == up && row > 0)
		{
			printLine();
			row--;
			col = (col > lenghtStr(lineStr[row])) ? lenghtStr(lineStr[row]) : col;
			printLine();
			update_cursor(col + colOffSet, row);
		}
		else if (letter == enter && row < height)
		{
			passRow(row);
			row++;
			col = 0;
			update_cursor(col + colOffSet, row);
			refreshMaxFileRows();
		}
		else if (letter == del)
		{
			if (col <= 0 && row > 0)
			{
	quitRow(row);
	row--;
	update_cursor(col + colOffSet, row);
	refreshMaxFileRows();
			}
			else if (col > 0)
			{
	eraseStr(lineStr[row], col - 1, 1);
	col--;
	printLine();
	update_cursor(col + colOffSet, row);
			}
		}
		else if (letter != 0 && letter < 128)
		{
			insertStr(lineStr[row], col, (char)letter);
			col++;
			printLine();
			update_cursor(col + colOffSet, row);
		}
		refresh();
	}
}
