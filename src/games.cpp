#include "headers/games.h"

#define width 80
#define height 30

#define widthPx 640
#define heightPx 480

void fillRect(uint32 x, uint32 y, uint32 w, uint32 h, char c)
{
	for (uint32 i = 0; i < h; i++)
	{
		gotoxy(x, y + i);
		for (uint32 j = 0; j < w; j++)
		{
			printf("%c", c);
		}
	}
}
void strokeRect(uint32 x, uint32 y, uint32 w, uint32 h, char c)
{
	for (uint32 i = 0; i <= w; i++)
	{
		gotoxy(x + i, y);
		printf("%c", c);
		gotoxy(x + i, y + h);
		printf("%c", c);
	}
	for (uint32 i = 0; i <= h; i++)
	{
		gotoxy(x, y + i);
		printf("%c", c);
		gotoxy(x + w, y + i);
		printf("%c", c);
	}
}
//////////////////// GAMES ///////////////////////

///// SNAKE //////
int8 SnakeTitle[7][80] = {
		" .M'''bgd                     `7MM              ",
		",MI    'Y                       MM              ",
		"`MMb.    `7MMpMMMb.   ,6'Yb.    MM  ,MP'.gP'Ya  ",
		"  `YMMNq.  MM    MM  8)   MM    MM ;Y  ,M'   Yb ",
		".     `MM  MM    MM   ,pm9MM    MM;Mm  8M'''''' ",
		"Mb     dM  MM    MM  8M   MM    MM `Mb.YM.    , ",
		"P'Ybmmd' .JMML  JMML.`Moo9^Yo..JMML. YA.`Mbmmd' ",
};

uint32 xArray[100], yArray[100];
uint32 arrayIndex = 3;
uint32 xFood = 10, yFood = 10;
uint8 dir = 3;
uint8 GameOver = 0;

void comprobeCoords()
{
	if (xArray[0] <= 1 || xArray[0] >= 38 || yArray[0] <= 1 || yArray[0] >= 28)
		GameOver = 1;
	if (arrayIndex > 3)
	{
		for (uint32 i = 0; i < arrayIndex; i++)
		{
			for (uint32 j = 0; j < arrayIndex; j++)
			{
	if (i == j)
		continue;
	if (xArray[i] == xArray[j] && yArray[i] == yArray[j])
	{
		GameOver = 1;
		break;
	}
			}
			if (GameOver)
	break;
		}
	}
}

void newFoodCoord()
{
	draw_rect(xFood * 16, yFood * 16, 16, 16, 0x02);
	srand(time());
	xFood = rand() % 34 + 3;
	yFood = rand() % 24 + 3;
}

void passCoords(uint8 nextDir)
{
	if (nextDir == 0 && xArray[0] != xArray[1])
	{
		dir = 0;
	}
	else if (nextDir == 1 && yArray[0] != yArray[1])
	{
		dir = 1;
	}
	else if (nextDir == 2 && xArray[0] != xArray[1])
	{
		dir = 2;
	}
	else if (nextDir == 3 && yArray[0] != yArray[1])
	{
		dir = 3;
	}

	for (uint32 i = arrayIndex; i > 0; i--)
	{
		xArray[i] = xArray[i - 1];
		yArray[i] = yArray[i - 1];
	}
	switch (dir)
	{
	case 0:
		yArray[0] = yArray[0] + 1;
		break;
	case 1:
		xArray[0] = xArray[0] - 1;
		break;
	case 2:
		yArray[0] = yArray[0] - 1;
		break;
	case 3:
		xArray[0] = xArray[0] + 1;
		break;
	}

	if (xArray[0] == xFood && yArray[0] == yFood)
	{
		arrayIndex++;
		newFoodCoord();
		draw_rect(0, 0, widthPx, 32, COLOR_WHITE);
		gotoxy(width / 2 - 4, 0);
		printf("Puntos %d", arrayIndex * 10 - 30);
	}
}

void printFood()
{
	draw_rect(xFood * 16, yFood * 16, 16, 16, COLOR_DARK_BLUE);
}

void printCoords()
{
	for (uint32 i = 0; i < arrayIndex + 1; i++)
	{
		draw_rect(xArray[i] * 16, yArray[i] * 16, 16, 16, COLOR_DARK_RED);
	}
}

void removeCoords()
{
	for (uint32 i = 0; i < arrayIndex + 1; i++)
	{
		draw_rect(xArray[i] * 16, yArray[i] * 16, 16, 16, COLOR_DARK_GREEN);
	}
}

uint32 timeSnake = 0;
uint32 maxTimeSnake = 0;

void snakeINIT()
{
	timeSnake = 0;
	arrayIndex = 3;
	GameOver = 0;
	xFood = 10;
	yFood = 10;
	cls(COLOR_BLACK);

	gotoxy(0, 0);
	changeColor(COLOR_WHITE);
	for (uint8 i = 0; i < 7; i++)
	{
		gotoxy(13, 9 + i);
		printf(SnakeTitle[i]);
		printf("/n");
	}
	gotoxy(21, 20);
	printf("Presione ESPACIO para continuar");

	refresh();

	while (getLastAsciiKey() != 32);
	eatKeyBuffered();
	draw_rect(0, 0, widthPx, heightPx, COLOR_WHITE);
	draw_rect(32, 32, widthPx - 64, heightPx - 64, COLOR_DARK_GREEN);

	changeColor(COLOR_BLACK);
	change_ground_color(0);
	gotoxy(width / 2 - 4, 0);
	printf("Puntos %d", arrayIndex * 10 - 30);

	uint8 ch0;
	uint8 nextDir = 3;

	xArray[0] = 20;
	yArray[0] = 10;
	xArray[1] = 21;
	yArray[1] = 10;
	xArray[2] = 22;
	yArray[2] = 10;

	printCoords();

	maxTimeSnake = (arrayIndex < 20) ? 20 - arrayIndex : 0;
	uint8 first = 1;
	while (1)
	{
		timeSnake++;
		ch0 = getLastAsciiKey();
		removeCoords();

		if (isKeyBuffered())
		{
			if (ch0 == escKeyCode)
			{
	cls(0x00);
	break;
			}
			if (ch0 == downKeyCode && dir != 2)
			{
	nextDir = 0;
			}
			else if (ch0 == leftKeyCode && dir != 3)
			{
	nextDir = 1;
			}
			else if (ch0 == upKeyCode && dir != 0)
			{
	nextDir = 2;
			}
			else if (ch0 == rightKeyCode && dir != 1)
			{
	nextDir = 3;
			}
			eatKeyBuffered();
		}
		if (!GameOver)
		{
			if (timeSnake == maxTimeSnake)
			{

	timeSnake = 0;
	passCoords(nextDir);
	comprobeCoords();
	maxTimeSnake = (arrayIndex < 20) ? 20 - arrayIndex : 00;
			}
			printFood();
			printCoords();
			draw_rect(0, 0, 16, 16, COLOR_WHITE);
		}
		else if (GameOver && first)
		{
			draw_rect(widthPx / 2 - 100 - 4 * 8, height / 2 * 16 - 66, 9 * 8 + 200, 16 + 100, COLOR_WHITE);
			gotoxy(width / 2 - 4, height / 2 - 1);
			printf("GAME OVER");
			first = 0;
		}
		refresh();
	}

	change_ground_color(1);
}

/////////////////////////////////////////////////////////////
uint32 desY=height/2;

void hardINIT(){
cls(COLOR_BLACK);
desY=heightPx/2;
COORD ballPos={widthPx/2,heightPx/2};
COORD ballVel={-5,-5};

draw_rect(5,desY-40,20,80,COLOR_DARK_RED);
refresh();
MouseDescriptor mouse;
while(true){
	
	getMouseDescriptor(&mouse);
	if(mouse.Pos.y!=desY){
	draw_rect(5,desY-40,20,80,COLOR_BLACK);
	refresh_rect(5,desY-40,20,80);
	desY=mouse.Pos.y;
	draw_rect(5,desY-40,20,80,COLOR_DARK_RED);
	//refresh();
	refresh_rect(5,desY-40,20,80);
	}
	}
}

////////////////////   HUB   ////////////////////////

uint32 menuSelectedGame = 0;
const uint32 menuMaxGames = 2;
int8 menuGameNames[menuMaxGames][80] = {"Snake", "Pong"};

void (*function[menuMaxGames])(void) = {
		snakeINIT,
		hardINIT};

uint8 l;
void gameSelected()
{
	eatKeyBuffered();
	while (1)
	{
		l = getLastAsciiKey();

		if (!isKeyBuffered())
			continue;
		eatKeyBuffered();
		if (l == escKeyCode)
			break;
		if (l == enterKeyCode)
		{
			function[menuSelectedGame]();
			break;
		}

		changeColor(COLOR_DARK_YELLOW);
		gotoxy(width / 2 - lenghtStr(menuGameNames[menuSelectedGame])/2, 7 + menuSelectedGame * 2);
		printf(menuGameNames[menuSelectedGame]);

		if (l == upKeyCode && menuSelectedGame > 0)
		{
			menuSelectedGame--;
		}
		else if (l == downKeyCode && menuSelectedGame < menuMaxGames - 1)
		{
			menuSelectedGame++;
		}

		changeColor(COLOR_DARK_GREEN);
		gotoxy(width / 2 - lenghtStr(menuGameNames[menuSelectedGame])/2, 7 + menuSelectedGame * 2);
		printf(menuGameNames[menuSelectedGame]);
		refresh();
	}
	change_ground_color(1);
}

void initGameHub()
{
	cls(0x38);
	change_ground_color(0);
	changeColor(0x1b);
	gotoxy(width / 2 - 4, 3);
	printf("Game-Hub");
	draw_rect(40, 40, widthPx - 80, heightPx - 80, COLOR_LIGHT_GRAY);
	draw_rect(48, 48, widthPx - 96, heightPx - 96, 0x3f);

	changeColor(COLOR_DARK_YELLOW);

	for (uint32 i = 0; i < menuMaxGames; i++)
	{
		gotoxy(width / 2 - lenghtStr(menuGameNames[i])/2, 7 + i * 2);
		printf(menuGameNames[i]);
	}

	refresh();

	gameSelected();
}
