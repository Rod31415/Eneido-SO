#include "headers/games.h"

#define width 80
#define height 80


void fillRect(uint32 x,uint32 y,uint32 w,uint32 h,char c){
for(uint32 i=0;i<h;i++){
gotoxy(x,y+i);
for(uint32 j=0;j<w;j++){
printf("%c",c);
}
}

}
void strokeRect(uint32 x,uint32 y,uint32 w,uint32 h,char c){
for(uint32 i=0;i<=w;i++){
  gotoxy(x+i,y);
  printf("%c",c);
  gotoxy(x+i,y+h);
  printf("%c",c);
}
for(uint32 i=0;i<=h;i++){
  gotoxy(x,y+i);
  printf("%c",c);
  gotoxy(x+w,y+i);
  printf("%c",c);
}

}
//////////////////// GAMES ///////////////////////


///// SNAKE ////// 

uint32 xArray[100],yArray[100];
uint32 arrayIndex=20;
uint32 xFood=10,yFood=10;

void passCoords(uint8 dir){

  for(uint32 i=arrayIndex;i>0;i--){
    xArray[i]=xArray[i-1];
    yArray[i]=yArray[i-1];
  }

switch (dir) {
  case 0:yArray[0]=yArray[0]+1;break;
  case 1:xArray[0]=xArray[0]-1;break;
  case 2:yArray[0]=yArray[0]-1;break;
  case 3:xArray[0]=xArray[0]+1;break;

}

if(xArray[0]==xFood&&yArray[0]==yFood){
  arrayIndex++;
  
}

}


void printFood(){
  gotoxy(xFood,yFood);
  printf("o");
}

void printCoords(){
  for(uint32 i=0;i<arrayIndex+1;i++){

    gotoxy(xArray[i],yArray[i]);
    printf("#");
  }
}

void removeCoords(){
 for(uint32 i=0;i<arrayIndex+1;i++){
   gotoxy(xArray[i],yArray[i]);
    printf(" ");
  }
}

void snakeINIT(){
  cls(0x1f);
  changeColor(0x1f);
uint8 ch0,ch1,ch2;
uint8 dir=3;
xArray[0]=20;
yArray[0]=10;
xArray[1]=21;
yArray[1]=10;
xArray[2]=22;
yArray[2]=10;


printCoords();


while(1){
for(uint32 i=0;i<10;i++){sleep();}
  ch1=ch0;
  ch2=inport(0x60);
  ch0=getKeyboardKey(ch2);
removeCoords();

if(ch0 != ch1){
if(ch0 == escKeyCode)break;
if(ch0 ==  downKeyCode && dir != 2){dir=0;}
else if(ch0 ==  leftKeyCode && dir != 3){dir=1;}
else if(ch0 ==    upKeyCode && dir != 0){dir=2;}
else if(ch0 == rightKeyCode && dir != 1){dir=3;}
}
passCoords(dir);
printFood();
printCoords();
refresh();

}

}



void patoINIT(){



}


void wolfensteinINIT(){
initVGA();




}

////////////////////   HUB   ////////////////////////

uint32 menuSelectedGame=0;
const uint32 menuMaxGames=3;
int8 menuGameNames[menuMaxGames][80]={"Snake","Pato, Pata y su pata","Wolfenstein"};

void (*function[menuMaxGames])(void)={
  snakeINIT,
  patoINIT,
  wolfensteinINIT
};

uint8 menuch, l=enterKeyCode,prL;
void gameSelected(){
  while(1){
  prL=l;
  menuch=inport(0x60);
  l=getKeyboardKey(menuch);
  
  if(l==prL)continue;
  if(l==escKeyCode)break;
  if(l==enterKeyCode){function[menuSelectedGame]();break;}

changeColor(0x7f);
gotoxy(12,5+menuSelectedGame*2);
printf(menuGameNames[menuSelectedGame]);

  if(l==upKeyCode&&menuSelectedGame>0){menuSelectedGame--;}
  else if(l==downKeyCode&&menuSelectedGame<menuMaxGames-1){menuSelectedGame++;}

changeColor(0x8f);
gotoxy(12,5+menuSelectedGame*2);
printf(menuGameNames[menuSelectedGame]);
  }

}

void initGameHub(){
cls(0x9f);
changeColor(0x9f);
gotoxy(width/2-4,1);
printf("Game-Hub");

changeColor(0x77);
  fillRect(10,3,60,19,(int8)219);
changeColor(0xFF);
strokeRect(10,3,60,19,'#');

changeColor(0x7f);

for(uint32 i=0;i<menuMaxGames;i++){
  gotoxy(12,5+i*2);
  printf(menuGameNames[i]);
}



gameSelected();
changeColor(0x0f);
cls(0x0f);
}


