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
  for(uint8 x=0;x<4;x++){
    for(uint8 y=0;y<4;y++){
      draw_pixel(xFood*4+x,yFood*4+y,0x1f);
    }
   }

}

void printCoords(){
 for(uint32 i=0;i<arrayIndex+1;i++){
   for(uint8 x=0;x<4;x++){
    for(uint8 y=0;y<4;y++){
      draw_pixel(xArray[i]*4+x,yArray[i]*4+y,0x1f);
    }
   }
}
}

void removeCoords(){
 for(uint32 i=0;i<arrayIndex+1;i++){
   for(uint8 x=0;x<4;x++){
    for(uint8 y=0;y<4;y++){
      draw_pixel(xArray[i]*4+x,yArray[i]*4+y,0);
    }
   }
}
}

void snakeINIT(){
  change_ground_color(1);
  cls(0x00);
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
for(uint32 i=0;i<20;i++){sleep();}
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
cls(0x00);
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


