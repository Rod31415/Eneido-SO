#include "headers/games.h"

#define width 80
#define height 25

#define widthPx 320
#define heightPx 200

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
uint32 arrayIndex=3;
uint32 xFood=10,yFood=10;

uint8 GameOver=0;

void comprobeCoords(){
   if(xArray[0]<=3||xArray[0]>=76||yArray[0]<=3||yArray[0]>=46)GameOver=1;
}

void newFoodCoord(){
  srand(time());
  xFood=rand()%60+10;
  yFood=rand()%20+10;
}

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
  newFoodCoord();
  draw_rect(120,0,160,12,0x2f);
  gotoxy(30,0);
  printf("Puntos %d",arrayIndex*10-30);
}

}


void printFood(){
  draw_rect(xFood*4,yFood*4,4,4,0x04);

}

void printCoords(){
 for(uint32 i=0;i<arrayIndex+1;i++){
draw_rect(xArray[i]*4,yArray[i]*4,4,4,0x01);
}
}

void removeCoords(){
 for(uint32 i=0;i<arrayIndex+1;i++){
draw_rect(xArray[i]*4,yArray[i]*4,4,4,0x02);
}
}

void snakeINIT(){
  arrayIndex=3;
  GameOver=0;
xFood=10;
yFood=10;
  cls(0x00);
  draw_rect(0,0,320,200,0x2f);
  draw_rect(12,12,296,176,0x02);

  changeColor(0x0);
  change_ground_color(0);
  gotoxy(30,0);
  printf("Puntos %d",arrayIndex*10-30);

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
for(uint32 i=0;i<(uint32)(1.0/(float)arrayIndex*80);i++){sleep();}
  ch1=ch0;
  ch2=inport(0x60);
  ch0=getKeyboardKey(ch2);
removeCoords();

if(ch0 != ch1){
if(ch0 == escKeyCode){cls(0x00);break;}
if(ch0 ==  downKeyCode && dir != 2){dir=0;}
else if(ch0 ==  leftKeyCode && dir != 3){dir=1;}
else if(ch0 ==    upKeyCode && dir != 0){dir=2;}
else if(ch0 == rightKeyCode && dir != 1){dir=3;}
}
if(!GameOver){
passCoords(dir);
comprobeCoords();
printFood();
printCoords();
draw_rect(0,0,4,4,0x2f);
}
else {
draw_rect(30*4-16,10*8-16,9*4+32,40,0x2f);
gotoxy(30,10);
printf("GAME OVER");

}
refresh();
}

  change_ground_color(1);
}



void patoINIT(){



}
/////////////////////////////////////////////////////////////
float xPl=2,yPl=2;
float anglePl=0;

uint8 map[10][10]={
  {1,1,1,1,1,1,1,1,1,1},
  {1,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,1,0,0,1},
  {1,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,1},
  {1,1,1,1,1,1,1,1,1,1},
};

float raycast(float angle);

void wolfensteinINIT(){
  cls(0x00);
for(float line=0;line<320;line++){
  float dis=raycast(anglePl+line/320.0);
  draw_line(line,heightPx/2-dis,line,heightPx/2+dis,0x0f);
}

}
float raycast(float angle){

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

changeColor(0x03);
gotoxy(width/2-10,7+menuSelectedGame*2);
printf(menuGameNames[menuSelectedGame]);

  if(l==upKeyCode&&menuSelectedGame>0){menuSelectedGame--;}
  else if(l==downKeyCode&&menuSelectedGame<menuMaxGames-1){menuSelectedGame++;}

changeColor(0x02);
gotoxy(width/2-10,7+menuSelectedGame*2);
printf(menuGameNames[menuSelectedGame]);
refresh();
  }
change_ground_color(1);
}

void initGameHub(){
cls(0x38);
  change_ground_color(0);
changeColor(0x1b);
gotoxy(width/2-10,3);
printf("Game-Hub");
draw_rect(40,40,240,120,0x07);
draw_rect(48,48,224,104,0x3f);

changeColor(0x03);

for(uint32 i=0;i<menuMaxGames;i++){
  gotoxy(width/2-10,7+i*2);
  printf(menuGameNames[i]);
}


refresh();

gameSelected();
}


