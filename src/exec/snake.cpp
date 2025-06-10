#include "snake.h"
uint8 lastChar=0,c=0;

uint8 dir=0;

uint32 x=10,y=10;

float a=0;
void snake(){
cls(0x30);
changeColor(0x30);
putChar(x,y,'#',0x30);
while(1){
a+=0.001;

gotoxy(0,0);

uint8 character = inport(0x60);
  lastChar = c;
  c = getKeyboardKey(character);
if(c==130&&lastChar!=130&&dir!=2){dir=0;}
if(c==131&&lastChar!=131&&dir!=3){dir=1;}
if(c==132&&lastChar!=132&&dir!=0){dir=2;}
if(c==133&&lastChar!=133&&dir!=1){dir=3;}



if(a>50){
  a=0;
putChar(x,y,' ',0x30);
switch (dir) {
  case 0:y++;break;
  case 1:x--;break;
  case 2:y--;break;
  case 3:x++;break;

};
putChar(x,y,219,0x30);
}
}


}
