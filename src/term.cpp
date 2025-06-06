#include "headers/term.h"

void drawOpenSource(uint32 flags, uint32 memLow, uint32 memHigh, uint32 sect){
	changeColor(0x02);
	printf("          #####/n");
	printf("      #############/n");
	printf("    #################/n");
	printf("  #####################/n");
	printf("  #########   #########/n");
	printf("#########       #########/n");
	printf("#########       #########/n");
	printf("#########       #########/n");
	printf("  ########     ########/n");
	printf("  ########     ########/n");
	printf("    #####       #####/n");
	printf("      ###       ###");

gotoxy(40,6);
printf("ENEIDO SO ");
gotoxy(40,7);
printf(" 32 bits ");
gotoxy(0,14);
uint32 address=0x0f00;
int32 a=*(int32*)address;
printf(" Sectores cargados: %d*512 bytes = %d bytes/n /n",sect,sect*512);
printf(" Memoria Disponible: %d KB /n /n",memLow+memHigh);
printf(" Banderas: %d ",flags);
}
uint32 xCursor=0,yCursor=0;
uint8 ch=0,s=0;
uint8 character=0;
int8 line[100];
uint8 indexLetter=0;
multiboot_info *mboot;
void init_term(multiboot_info *mb){
mboot=mb;
	cls(0x0f);
drawOpenSource(mboot->flags, mboot->mem_lower, mboot->mem_upper, mboot->mods_count);
gotoxy(0,20);
changeColor(0x0f);
printf(">");
xCursor=1;
yCursor=20;
update_cursor(xCursor,yCursor);
}

void detectCommands(){
if(strcmp(line,"clear",5)==0){
  init_term(mboot);
}
else if(strcmp(line,"echo",4)==0){
  backspace();
printf((line+5));
new_line_term();
xCursor=1;
if(yCursor<25)yCursor++;
update_cursor(xCursor,yCursor);}

else if(strcmp(line,"help",4)==0){
backspace();
printf("  Solo tenemos los siguientes comandos: /n /n");
printf("   echo - Mostrar un mensaje en la pantalla /n");
printf("   clear - Reiniciar la terminal /n");
printf("   help - Abrir esta ventana /n");
 new_line_term();
xCursor=1;

if(yCursor<25)yCursor++;

update_cursor(xCursor,yCursor);

}

else{
  backspace();
  printf("Uhh ni idea, no conozco ese comando ERROR: 01");
  new_line_term();
xCursor=1;
if(yCursor<25)yCursor++;
update_cursor(xCursor,yCursor);
  }

for(uint8 i=0;i<100;i++){
  line[i]=0;
}

}


void loop_term(){

	character=inport(0x60);
	ch=s;
	s=getKeyboardKey(character);
	if(s==128&&ch!=128){new_line_term();xCursor=1;if(yCursor<25)yCursor++;update_cursor(xCursor,yCursor);detectCommands();}
		if(s==129&&ch!=129){backspace();indexLetter--;xCursor--;update_cursor(xCursor,yCursor);}
  if(s!=ch&&s!=0&&s<128){	
	printf("%c",(char)s);line[indexLetter]=s;indexLetter++;xCursor++;update_cursor(xCursor,yCursor);}
		

}

void new_line_term(){
printf("/n>n");
indexLetter=0;
}

