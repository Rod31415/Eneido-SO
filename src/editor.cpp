#include "headers/editor.h"


uint32 col=0,row=0;

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

bool kbhit(){

uint8 status=inport(0x64);

return (status&1);
}

 uint32 localX=0,localY=0;
DIR localFile; 
void initEditor(DIR file)
{

col=0;
row=0;

  for(int i=0;i<height;i++){
    for(int j=0;j<width;j++){
      lineStr[i][j]=0;
    }
  }
  localFile=file;
	cls(0x00);
uint8 buff[localFile.size*512];
for(uint32 i=0;i<localFile.size;i++){
readCluster(&buff[i*512],localFile.firstcluster*8+localFile.directions+i);
}
uint32 subu=0,upu=0;
  for(uint32 u=0;u<localFile.size*512;u++){
    subu=u%width;
  lineStr[upu][subu]=(int8)buff[u];
  if(subu==width-1){upu++;}
}
changeColor(0x0f);
for(uint32 i=0;i<height;i++){
gotoxy(0,i);
printf(lineStr[i]);
}
//update_cursor(0,0);
draw_rect(0,(height-1)*16+8,width*8,24,0x0f);
change_ground_color(0);
changeColor(0x00);
gotoxy(1,29);
printf("FILE:");
printf(localFile.name);
gotoxy(30,29);
printf("EDITOR by Rodrigo");
gotoxy(62,29);
printf("ESC for save");
change_ground_color(1);
refresh();
gotoxy(0,0);
changeColor(0x0f);
loopEditor();

}
uint8 charac, letter=enter,prevLetter=enter;

void printLine(){
uint8 buffer[41];
      memset((uint32)buffer,32,40);
      buffer[41]=0;
      gotoxy(0,row);
      printf((int8 *)buffer);
      gotoxy(0,row);
      printf(lineStr[row]);
}

void checkSize(){
  uint32 size=0;
for(uint32 h=0;h<1000;h++){
if(lineStr[h][0]==0){size=h*80;break;}
}
localFile.size=(uint32)(size/512)+1;
modifyFile(localFile.name,localFile);
}

void loopEditor()
{
			update_cursor(col, row);
	while (1)
	{
		charac = inport(0x60);
	  prevLetter=letter;
		letter = getKeyboardKey(charac);
    if(letter==prevLetter){continue;}
    if (letter == esc){

checkSize();
uint8 buff[localFile.size*512];
uint32 subu=0,upu=0;
  for(uint32 u=0;u<localFile.size*512;u++){
    subu=u%80;
  buff[u]=(uint8)lineStr[upu][subu];
  if(subu==79){upu++;}
}
for(uint32 i=0;i<localFile.size;i++)
writeCluster(&buff[i*512],localFile.firstcluster*8+localFile.directions+i);

      break;
    }
    uint32 lgtStr=lenghtStr(lineStr[row]);
		     if(letter == left  && col>0       ){col--;printLine();update_cursor(col, row);}
    else if(letter == right && col<lgtStr  ){col++;printLine();update_cursor(col, row);}
    else if(letter == down  && row<height  ){printLine();row++;col=(col > lenghtStr(lineStr[row]) )?lenghtStr(lineStr[row]):col; printLine();update_cursor(col, row);}
    else if(letter == up    && row>0       ){printLine();row--;col=(col > lenghtStr(lineStr[row]) )?lenghtStr(lineStr[row]):col; printLine();update_cursor(col, row);}
    else if(letter == enter && row<height  ){printLine();row++;col=0; printLine();update_cursor(col,row);}
    else if(letter == del   && col > 0     ){
      eraseStr(lineStr[row],col-1,1);
      col--;
      printLine();
			update_cursor(col, row);
          }
    else if(letter != 0 && letter<128)
		{
      insertStr(lineStr[row],col,(char)letter);
      col++; 
      printLine();
			update_cursor(col, row);
    }
  refresh();
  }
}

