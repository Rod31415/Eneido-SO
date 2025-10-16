#include "headers/editor.h"

uint32 maxRows = 0;
#define colOffSet 2
#define rowOffSet 0

#define down 130
#define left 131
#define up 132
#define right 133
#define esc 135
#define del 129
#define enter 128

#define height 29
#define width 80

#define COMBINE_WORD(msb, lsb) (((uint32)(msb) << 16) | lsb)


typedef struct {
  char **lines;
  int row;
  int col;
}TextBuffer;


TextBuffer GeneralTextBuff;

void insertStrLine(char* buff,int index,char letter){
  for(int i=80;i>index;i--){
    buff[i]=buff[i-1];
  }
  buff[index]=letter;
}

bool kbhit()
{

	uint8 status = inport(0x64);

	return (status & 1);
}
uint8 letter;

uint32 localX = 0, localY = 0;
KFILE *localFile;

void drawSide(int max)
{
	draw_rect(0, 0, 16, height * 16, COLOR_LIGHT_GRAY);
	changeColor(COLOR_BLACK);
	change_ground_color(false);
	for (uint32 i = 0; i <= max ; i++)
	{
		draw_rect(0, i * 16, 16, 16, COLOR_LIGHT_GRAY);
		gotoxy((i < 10) ? 1 : 0, i);
		printf("%d", i);
	}
	changeColor(COLOR_WHITE);
	change_ground_color(1);
}
uint32 address = 0;
void drawBottom()
{
	draw_rect(0, (height - 1) * 16 + 8, width * 8, 24, COLOR_WHITE);
	change_ground_color(false);
	changeColor(COLOR_BLACK);
	gotoxy(1, 29);
	printf("FILE: %d", address);
	// printf(localFile->Name);
	gotoxy(36, 29);
	printf("EDITOR");
	gotoxy(62, 29);
	printf("ESC for save");
	change_ground_color(1);
	refresh();
	gotoxy(2, 0);
	changeColor(COLOR_WHITE);
}
char* buff;

void initEditor(char *filename)
{
	localFile = kopen(filename,O_RDWR);
	address = (COMBINE_WORD(localFile->entry->FstClusHI, localFile->entry->FstClusLO) * 512 + 0x800000);
	if (localFile == (KFILE*)-1)
		return;
	cls(COLOR_BLACK);
  buff=(char*)kalloc(localFile->entry->FileSize*512);
	kgets(buff, 512, localFile);

  GeneralTextBuff.lines=(char**)kalloc(512);
  GeneralTextBuff.row=0;
  GeneralTextBuff.col=0;
  

  GeneralTextBuff.lines[0]=(char*)kalloc(80);
  gotoxy(colOffSet,0);
	for (uint32 u = 0; u < localFile->entry->FileSize * 512; u++)
	{
    GeneralTextBuff.lines[GeneralTextBuff.row][GeneralTextBuff.col++]=buff[u];
    if(buff[u]==0xA||buff[u]==0xD){GeneralTextBuff.lines[GeneralTextBuff.row][GeneralTextBuff.col-1]=0;GeneralTextBuff.row++;GeneralTextBuff.col=0;GeneralTextBuff.lines[GeneralTextBuff.row]=(char*)kalloc(80);}
    //if(GeneralTextBuff.lines[GeneralTextBuff.line_count][charIndex-1]!=0xA)
    if(buff[u]==0){GeneralTextBuff.lines[GeneralTextBuff.row+1][0]=0;break;}

	}
	changeColor(COLOR_WHITE);
  kfree(buff);
  maxRows=GeneralTextBuff.row;

  GeneralTextBuff.row=0;
  GeneralTextBuff.col=0;


  /*for(int i=0;i<16;i++){
  printf("%d ",buff[i]);
  }*/

  while(GeneralTextBuff.row<=maxRows){
    gotoxy(colOffSet,GeneralTextBuff.row);
    for(uint32 i=0;i<80;i++){
      //printf("%d ",GeneralTextBuff.lines[GeneralTextBuff.row][i]);
      printChr(GeneralTextBuff.lines[GeneralTextBuff.row][i]);
      if(GeneralTextBuff.lines[GeneralTextBuff.row][i]==0){break;}
    }
  GeneralTextBuff.row++;
  }

	drawSide(maxRows);
	drawBottom();

GeneralTextBuff.row=0;
GeneralTextBuff.col=0;


	update_cursor(colOffSet,0);

	letter = enter;
	loopEditor();
}

void printAll(){
int auxA=GeneralTextBuff.row,auxB=GeneralTextBuff.col;
GeneralTextBuff.row=0;
GeneralTextBuff.col=0;

while(GeneralTextBuff.row<=maxRows){
draw_rect(colOffSet*8,GeneralTextBuff.row*16,(width- colOffSet)*8,16,COLOR_BLACK);

    gotoxy(colOffSet,GeneralTextBuff.row);
    for(uint32 i=0;i<80;i++){
      //printf("%d ",GeneralTextBuff.lines[GeneralTextBuff.row][i]);
      printChr(GeneralTextBuff.lines[GeneralTextBuff.row][i]);
      if(GeneralTextBuff.lines[GeneralTextBuff.row][i]==0){break;}
    }
  GeneralTextBuff.row++;
  }


GeneralTextBuff.row=auxA;
GeneralTextBuff.col=auxB;
}


void printLine()
{
  draw_rect(colOffSet*8,GeneralTextBuff.row*16,(width- colOffSet)*8,16,COLOR_BLACK);
gotoxy(colOffSet,GeneralTextBuff.row);
    for(uint32 i=0;i<80;i++){
      
      printChr(GeneralTextBuff.lines[GeneralTextBuff.row][i]);
      if(GeneralTextBuff.lines[GeneralTextBuff.row][i]==0){break;}
}
}

void splitAtIndex(char* buff0, char* buff1, int index){
for(int i=index;i<80;i++){
  buff1[i-index]=buff0[i];
  buff0[i]=0;
}
}

void combineAtIndex(char* buff0, char* buff1, int index){
for(int i=index;i<80;i++){
  buff0[i]=buff1[i-index];
  buff1[i-index]=0;
}
}

void removeLinePointer(int row){

  for(int i=row;i<maxRows+1;i++){
    GeneralTextBuff.lines[i]=GeneralTextBuff.lines[i+1];
  }
maxRows--;
  kfree(GeneralTextBuff.lines[maxRows+1]);

  //GeneralTextBuff.lines[row][0]=0xFE;

  //for(int i=0;i<80;i++)
  //GeneralTextBuff.lines[row+1][i]=0x0;
}




void passLinePointer(int row){
  int max=maxRows+2;

  GeneralTextBuff.lines[row+1]=(char*)kalloc(80);
  for(int i=max;i>row+1;i--){
    GeneralTextBuff.lines[i]=GeneralTextBuff.lines[i-1];
  }
maxRows++;

  //GeneralTextBuff.lines[row][0]=0xFE;

  for(int i=0;i<80;i++)
  GeneralTextBuff.lines[row+1][i]=0x0;
}

void loopEditor()
{
	update_cursor(GeneralTextBuff.col + colOffSet, GeneralTextBuff.row);
	eatKeyBuffered();
	while (1)
	{
		letter = getLastAsciiKey();
		if (!isKeyBuffered())
			continue;
		eatKeyBuffered();

		if (letter == esc)
		{

			uint32 u;
			bool ret = false;
      buff=(char*)kalloc(localFile->entry->FileSize * 512);
GeneralTextBuff.row=0;
GeneralTextBuff.col=0;

			for (u = 0; u < localFile->entry->FileSize * 512; u++)
			{
          if(GeneralTextBuff.row>maxRows){
            buff[u-1]=0;
            break;
          }
          if(GeneralTextBuff.lines[GeneralTextBuff.row][GeneralTextBuff.col]==0){
            buff[u]=0xA;
            GeneralTextBuff.row++;
            GeneralTextBuff.col=0;
            continue;
          }
          buff[u]=GeneralTextBuff.lines[GeneralTextBuff.row][GeneralTextBuff.col++];
			}
			kseek(0, localFile);
			kputs(buff, u+1, localFile);
		  break;
		}
		if (letter == left && GeneralTextBuff.col > 0)
		{
			GeneralTextBuff.col--;
			printLine();
			update_cursor(GeneralTextBuff.col + colOffSet, GeneralTextBuff.row);
		}
		else if (letter == right && GeneralTextBuff.col < lenghtStr(GeneralTextBuff.lines[GeneralTextBuff.row]))
		{
			GeneralTextBuff.col++;
			printLine();
			update_cursor(GeneralTextBuff.col + colOffSet, GeneralTextBuff.row);
		}
		else if (letter == down && GeneralTextBuff.row < maxRows )
		{
			printLine();
			GeneralTextBuff.row++;
			GeneralTextBuff.col = (GeneralTextBuff.col > lenghtStr(GeneralTextBuff.lines[GeneralTextBuff.row])) ? lenghtStr(GeneralTextBuff.lines[GeneralTextBuff.row]) : GeneralTextBuff.col;
			printLine();
			update_cursor(GeneralTextBuff.col + colOffSet, GeneralTextBuff.row);
		}
		else if (letter == up && GeneralTextBuff.row > 0)
		{
			printLine();
			GeneralTextBuff.row--;
			GeneralTextBuff.col = (GeneralTextBuff.col > lenghtStr(GeneralTextBuff.lines[GeneralTextBuff.row])) ? lenghtStr(GeneralTextBuff.lines[GeneralTextBuff.row]) : GeneralTextBuff.col;
			printLine();
			update_cursor(GeneralTextBuff.col + colOffSet, GeneralTextBuff.row);
		}
		else if (letter == enter && GeneralTextBuff.row < height)
		{

      passLinePointer(GeneralTextBuff.row);
      splitAtIndex(GeneralTextBuff.lines[GeneralTextBuff.row],GeneralTextBuff.lines[GeneralTextBuff.row+1],GeneralTextBuff.col);
      drawSide(maxRows);
			GeneralTextBuff.row++;
			GeneralTextBuff.col = 0;
      printAll();
			update_cursor(GeneralTextBuff.col + colOffSet, GeneralTextBuff.row);

		}
		else if (letter == del)
		{
			if (GeneralTextBuff.col == 0 && GeneralTextBuff.row > 0)
			{
        combineAtIndex(GeneralTextBuff.lines[GeneralTextBuff.row-1],GeneralTextBuff.lines[GeneralTextBuff.row],lenghtStr(GeneralTextBuff.lines[GeneralTextBuff.row-1]));
				removeLinePointer(GeneralTextBuff.row);
        drawSide(maxRows);
				GeneralTextBuff.row--;
				update_cursor(GeneralTextBuff.col + colOffSet, GeneralTextBuff.row);
        draw_rect(colOffSet*8,(maxRows+1)*16,(width-colOffSet)*8,16,COLOR_BLACK);

        printAll();
        }
			else if (GeneralTextBuff.col > 0)
			{
				eraseStr(GeneralTextBuff.lines[GeneralTextBuff.row], GeneralTextBuff.col - 1, 1);
				GeneralTextBuff.col--;
				printLine();
				update_cursor(GeneralTextBuff.col + colOffSet, GeneralTextBuff.row);
			}
		}
		else if (letter != 0 && letter < 128)
		{
			insertStrLine(GeneralTextBuff.lines[GeneralTextBuff.row], GeneralTextBuff.col, (char)letter);
			GeneralTextBuff.col++;
			printLine();
			update_cursor(GeneralTextBuff.col + colOffSet, GeneralTextBuff.row);
		}
		refresh();
	}
}
