#include "headers/keyboard.h"

uint8 keyBuffered[20];
uint8 keyStackBuffer = 0;
uint8 lastKeyBuffered[20];
uint8 lastAsciiKeyBuffered[20];

uint8 shiftOn = 0;

void initKeyboard()
{
	memset((uint32)keyBuffered, 0, sizeof(keyBuffered));
	memset((uint32)lastKeyBuffered, 0, sizeof(lastKeyBuffered));
	memset((uint32)lastAsciiKeyBuffered, 0, sizeof(lastAsciiKeyBuffered));
	outport(0x60, 0xf4);
}

void eatKeyBuffered()
{
	if (keyStackBuffer > 0)
	{
		for (uint32 i = 0; i < keyStackBuffer; i++)
		{
			lastAsciiKeyBuffered[i] = lastAsciiKeyBuffered[i + 1];
			lastKeyBuffered[i] = lastKeyBuffered[i + 1];
		}
		keyBuffered[keyStackBuffer] = false;
		keyStackBuffer--;
	}
}

uint8 getLastAsciiKey()
{
	return lastAsciiKeyBuffered[0];
}

uint8 getLastKeyCode()
{
	return lastKeyBuffered[0];
}

uint8 isKeyPressed()
{
	return keyBuffered[0];
}

uint8 keyboard_read();
uint8 getKeyboardKey(uint8 input);

void keyboard_irq_handler(struct regs *r)
{

	lastKeyBuffered[keyStackBuffer] = keyboard_read();
	if (lastKeyBuffered[keyStackBuffer] == 42)
		shiftOn = 1;
	if (lastKeyBuffered[keyStackBuffer] == 170)
		shiftOn = 0;

	if (lastKeyBuffered[keyStackBuffer] < 128)
	{
		lastAsciiKeyBuffered[keyStackBuffer] = getKeyboardKey(lastKeyBuffered[keyStackBuffer]);
		keyBuffered[keyStackBuffer] = true;
		keyStackBuffer++;
	}
}

uint8 keyboard_read()
{
	return inport(0x60);
}
uint8 getKeyboardKey(uint8 input)
{

	switch (input)
	{
	case 27:
		return (uint8)((shiftOn) ? '*' : '+');
		break;
	case 26:
		return (uint8)((shiftOn) ? 126 : 94);
		break;
	case 40:
		return (uint8)((shiftOn) ? '[' : '{');
		break;
	case 43:
		return (uint8)((shiftOn) ? ']' : '}');
		break;
	case 12:
		return (uint8)((shiftOn) ? '?' : 39);
		break;
	case 13:
		return (uint8)((shiftOn) ? '¡' : '¿');
		break;
	case 41:
		return (uint8)((shiftOn) ? '°' : '|');
		break;
	case 30:
		return (uint8)((shiftOn) ? 'A' : 'a');
		break;
	case 48:
		return (uint8)((shiftOn) ? 'B' : 'b');
		break;
	case 46:
		return (uint8)((shiftOn) ? 'C' : 'c');
		break;
	case 32:
		return (uint8)((shiftOn) ? 'D' : 'd');
		break;
	case 18:
		return (uint8)((shiftOn) ? 'E' : 'e');
		break;
	case 33:
		return (uint8)((shiftOn) ? 'F' : 'f');
		break;
	case 34:
		return (uint8)((shiftOn) ? 'G' : 'g');
		break;
	case 35:
		return (uint8)((shiftOn) ? 'H' : 'h');
		break;
	case 23:
		return (uint8)((shiftOn) ? 'I' : 'i');
		break;
	case 36:
		return (uint8)((shiftOn) ? 'J' : 'j');
		break;
	case 37:
		return (uint8)((shiftOn) ? 'K' : 'k');
		break;
	case 38:
		return (uint8)((shiftOn) ? 'L' : 'l');
		break;
	case 50:
		return (uint8)((shiftOn) ? 'M' : 'm');
		break;
	case 49:
		return (uint8)((shiftOn) ? 'N' : 'n');
		break;
	case 39:
		return (uint8)((shiftOn) ? 'N' : 'n');
		break;
	case 24:
		return (uint8)((shiftOn) ? 'O' : 'o');
		break;
	case 25:
		return (uint8)((shiftOn) ? 'P' : 'p');
		break;
	case 16:
		return (uint8)((shiftOn) ? 'Q' : 'q');
		break;
	case 19:
		return (uint8)((shiftOn) ? 'R' : 'r');
		break;
	case 31:
		return (uint8)((shiftOn) ? 'S' : 's');
		break;
	case 20:
		return (uint8)((shiftOn) ? 'T' : 't');
		break;
	case 22:
		return (uint8)((shiftOn) ? 'U' : 'u');
		break;
	case 47:
		return (uint8)((shiftOn) ? 'V' : 'v');
		break;
	case 17:
		return (uint8)((shiftOn) ? 'W' : 'w');
		break;
	case 45:
		return (uint8)((shiftOn) ? 'X' : 'x');
		break;
	case 21:
		return (uint8)((shiftOn) ? 'Y' : 'y');
		break;
	case 44:
		return (uint8)((shiftOn) ? 'Z' : 'z');
		break;
	case 2:
		return (uint8)((shiftOn) ? '!' : '1');
		break;
	case 3:
		return (uint8)((shiftOn) ? '"' : '2');
		break;
	case 4:
		return (uint8)((shiftOn) ? '#' : '3');
		break;
	case 5:
		return (uint8)((shiftOn) ? '$' : '4');
		break;
	case 6:
		return (uint8)((shiftOn) ? '%' : '5');
		break;
	case 7:
		return (uint8)((shiftOn) ? '&' : '6');
		break;
	case 8:
		return (uint8)((shiftOn) ? '/' : '7');
		break;
	case 9:
		return (uint8)((shiftOn) ? '(' : '8');
		break;
	case 10:
		return (uint8)((shiftOn) ? ')' : '9');
		break;
	case 11:
		return (uint8)((shiftOn) ? '=' : '0');
		break;
	case 52:
		return (uint8)((shiftOn) ? ':' : '.');
		break;
	case 51:
		return (uint8)((shiftOn) ? ');' : ',');
		break;
	case 53:
		return (uint8)((shiftOn) ? '_' : '-');
		break;
	case 86:
		return (uint8)((shiftOn) ? '>' : '<');
	case 14: // TAB
		return 129;
		break;
	case 28:
		return 128;
		break;
	case 80: // down
		return 130;
		break;
	case 75: // left
		return 131;
		break;
	case 72: // up
		return 132;
		break;
	case 77: // right
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
