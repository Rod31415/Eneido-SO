#include "headers/keyboard.h"


uint8 keyBuffered[20];
uint8 keyStackBuffer=0;
uint8 lastKeyBuffered[20];
uint8 lastAsciiKeyBuffered[20];




uint8 shiftOn = 0;

void initKeyboard(){
memset((uint32)keyBuffered,0,sizeof(keyBuffered));
memset((uint32)lastKeyBuffered,0,sizeof(lastKeyBuffered));
memset((uint32)lastAsciiKeyBuffered,0,sizeof(lastAsciiKeyBuffered));
}

void eatKeyBuffered(){
	if(keyStackBuffer>0){
		for(uint32 i=0;i<keyStackBuffer;i++){
			lastAsciiKeyBuffered[i]=lastAsciiKeyBuffered[i+1];
			lastKeyBuffered[i]=lastKeyBuffered[i+1];
		}
		keyBuffered[keyStackBuffer]=false;
		keyStackBuffer--;
	}
}

uint8 getLastAsciiKey(){
    return lastAsciiKeyBuffered[0];
}

uint8 getLastKeyCode(){
    return lastKeyBuffered[0];
}

uint8 isKeyPressed(){
	return keyBuffered[0];
}

uint8 keyboard_read();
uint8 getKeyboardKey(uint8 input);

void keyboard_irq_handler(struct regs *r){

    
    lastKeyBuffered[keyStackBuffer]=keyboard_read();
	if (lastKeyBuffered[keyStackBuffer] == 42)
		shiftOn = 1;
	if (lastKeyBuffered[keyStackBuffer] == 170)
		shiftOn = 0;

	if(lastKeyBuffered[keyStackBuffer]<128){
	lastAsciiKeyBuffered[keyStackBuffer]=getKeyboardKey(lastKeyBuffered[keyStackBuffer]);
	keyBuffered[keyStackBuffer]=true;
	keyStackBuffer++;}

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
  case 86:
    return (shiftOn) ? '>' : '<';
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
