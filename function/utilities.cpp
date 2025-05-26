#include "../headers/utilities.h"

char lastkey=' ';

uint8 inport(uint16 port){
uint8 rv;
__asm__ __volatile__ ("inb %1, %0" : "=a"(rv) : "dN"(port));
return rv;
}

void outport(uint16 port,uint8 data){
__asm__ __volatile__ ("outb %1, %0" : : "dN"(port),"a"(data));
}

void bflush(){
uint8 i=1;
while(i++)inport(0x60);

}

uint8 keyboard_read(){

//while(!(inport(0x64)&1));
return inport(0x60);
return 0;
}
char recordLastKey(uint8 input){
return getKeyboardKey(input);
}
char getKeyboardKey(uint8 input){

switch(input){
	case 30:return 'a';break;
	case 48:return 'b';break;
	case 46:return 'c';break;
	case 32:return 'd';break;
	case 18:return 'e';break;
	case 33:return 'f';break;
	case 34:return 'g';break;
	case 35:return 'h';break;
	case 23:return 'i';break;
	case 36:return 'j';break;
	case 37:return 'k';break;
	case 38:return 'l';break;
	case 50:return 'm';break;
	case 49:return 'n';break;
	case 39:return 'n';break;
	case 24:return 'o';break;
	case 25:return 'p';break;
	case 16:return 'q';break;
	case 19:return 'r';break;
	case 31:return 's';break;
	case 20:return 't';break;
	case 22:return 'u';break;
	case 47:return 'v';break;
	case 17:return 'w';break;
	case 45:return 'x';break;
	case 21:return 'y';break;
	case 44:return 'z';break;
	case 57:return ' ';break;	
}
return 0;
}


