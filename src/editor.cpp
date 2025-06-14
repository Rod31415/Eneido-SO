#include "headers/editor.h"


uint32 col=0,row=0;

#define down 130
#define left 131
#define up 132
#define right 133
#define esc 135
#define del 129
#define enter 128

#define height 24
#define width 80

char lineStr[height][width];

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
	cls(0x0f);
uint8 buff[localFile.size*512];
readCluster(buff,localFile.firstcluster*8+localFile.directions);
uint32 subu=0,upu=0;
  for(uint32 u=0;u<localFile.size*512;u++){
    subu=u%width;
  lineStr[upu][subu]=(int8)buff[u];
  if(subu==width-1){upu++;}
}
for(uint32 i=0;i<height;i++){

gotoxy(0,i);
printf(lineStr[i]);
}
update_cursor(0,0);

changeColor(0x77);
gotoxy(0,24);
for(uint32 i=0;i<width;i++){
  printf(" ");
}
  changeColor(0x70);
gotoxy(0,24);
printf(localFile.name);
gotoxy(32,24);
printf("EDITOR by Rodrigo");
changeColor(0x0f);
loopEditor();

}
uint8 charac, letter=enter,prevLetter=enter;
void loopEditor()
{
	while (1)
	{
    if(kbhit){
		charac = inport(0x60);
	  prevLetter=letter;
		letter = getKeyboardKey(charac);
    if (letter == esc&&prevLetter!=esc){

uint8 buff[localFile.size*512];
uint32 subu=0,upu=0;
  for(uint32 u=0;u<localFile.size*512;u++){
    subu=u%80;
  buff[u]=(uint8)lineStr[upu][subu];
  if(subu==79){upu++;}
}
writeCluster(buff,localFile.firstcluster*8+localFile.directions);

      break;
    }
    uint32 lgtStr=lenghtStr(lineStr[row]);
		     if(letter == left  && prevLetter != left  && col>0       ){col--; update_cursor(col, row);}
    else if(letter == right && prevLetter != right && col<lgtStr  ){col++; update_cursor(col, row);}
    else if(letter == down  && prevLetter != down  && row<height  ){row++;col=(col > lenghtStr(lineStr[row]) )?lenghtStr(lineStr[row]):col; update_cursor(col, row);}
    else if(letter == up    && prevLetter != up    && row>0       ){row--;col=(col > lenghtStr(lineStr[row]) )?lenghtStr(lineStr[row]):col; update_cursor(col, row);}
    else if(letter == enter && prevLetter != enter && row<height  ){row++;col=0; update_cursor(col,row);}
    else if(letter == del   && prevLetter != del   && col > 0     ){
      eraseStr(lineStr[row],col-1,1);
      col--;
			update_cursor(col, row);

      uint8 buffer[81];
      memset((uint32)buffer,32,80);
      buffer[81]=0;
      gotoxy(0,row);
      printf((int8 *)buffer);
      gotoxy(0,row);
      printf(lineStr[row]);

    }
    else if(letter != prevLetter && letter != 0 && letter<128)
		{

      insertStr(lineStr[row],col,(char)letter);
      col++; 
			update_cursor(col, row);
     
      uint8 buffer[81];
      memset((uint32)buffer,32,80);
      buffer[81]=0;
      gotoxy(0,row);
      printf((int8 *)buffer);
      gotoxy(0,row);
      printf(lineStr[row]);

    }
	}

  }
}

