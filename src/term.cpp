#include "headers/term.h"


uint8 boolOnTest=0;

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
gotoxy(0,13);

changeColor(0x0E);
printf("   Por Rodrigo Bustos ");
changeColor(0x02);
gotoxy(0,15);
printf(" Memoria Disponible: %d KB /n",memLow+memHigh);
printf(" Banderas: %d /n /n",flags);
changeColor(0x0f);
printf(" Escribi 'help' para ver los comandos/n");

}



uint32 xCursor=0,yCursor=0;
uint8 ch=0,s=0;
uint8 character=0;
int8 line[100];
uint8 indexLetter=0;
multiboot_info *mboot;
uint32 argc;
int8 argv[10][40];


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

void formatCommand(){
argc=1;
uint8 active=1;
uint8 comm=0;
int i;
for(int j=0;j<10;j++){
for(i=0;i<40;i++){
argv[j][i]=0;
}}
i=0;
int a=0;

uint8 cn=0;
do{
  cn=0;
	if(line[i]=='"'){comm=!comm;}
  else if(line[i]==' '&&!comm){
    if(active==0){
	argc++;active=1;}
    cn=1;a=i+1;}
  
  else {active=0;}
  if(!cn)
argv[argc-1][i-a]=line[i];

}while(line[++i]!=0);
}

void detectCommands(){
if(argv[0][0]==0){ return; }

  if(argc==1&&strcmp(argv[0],"init",4)==0){
  init_term(mboot);}
  
  else if(strcmp(argv[0],"echo",4)==0){
  backspace();
  for(int i=0;i<40;i++){(argv[1][i]=='"')?argv[1][i]=' ':0;}
  printf((argc==2)?argv[1]:"ERROR : Escribe un mensaje despues del comando");
  new_line_term();
  xCursor=1;
    if(yCursor<24)yCursor++;
  update_cursor(xCursor,yCursor);}

else if(argc==1&&strcmp(argv[0],"help",4)==0){
backspace();
printf("  Solo tenemos los siguientes comandos: /n /n");
printf("   echo    - Mostrar un mensaje en la pantalla /n");
printf("   init    - Reiniciar la terminal /n");
printf("   help    - Abrir esta ventana /n");
printf("   colors  - Abrir una prueba de colores /n");
printf("   license - Muesta la licencia y creditos del proyecto /n");
printf("   clear   - Borrar la pantalla de la terminal /n");
 new_line_term();
xCursor=1;
for(int i=0;i<9;i++)
if(yCursor<24)yCursor++;

update_cursor(xCursor,yCursor);

}

else if(argc==1&&strcmp(argv[0], "colors",6)==0){
boolOnTest=1;

cls(0x0f);
gotoxy(0,0);
  for(uint16 i=0;i<256;i++){
changeColor(i);
printf("Prueba ");

  if(i%11==10)printf("/n");
}

}

else if(argc==1&&strcmp(argv[0], "license",7)==0){
backspace();
printf("/n   GNU General Public License v3.0 /n");
printf("     Hecho por Rodrigo Bustos /n");
printf(" Puedes acceder al codigo fuente yendo a :/n");
printf("   https://github.com/Rod31415/Eneido-SO/n /n");
  new_line_term();
xCursor=1;

for(int i=0;i<7;i++)
if(yCursor<24)yCursor++;

update_cursor(xCursor,yCursor);
 
}
else if(argc==1&&strcmp(argv[0], "clear",5)==0){
	cls(0x0f);
  gotoxy(0,0);
changeColor(0x0f);
printf(">");
xCursor=1;
yCursor=0;
update_cursor(xCursor,yCursor);

}

else if(argc==1&&strcmp(argv[0],"ls",2)==0){
backspace();
  int h=readFiles();

changeColor(0x0f);
  xCursor=1;

for(int i=0;i<h+2;i++)
if(yCursor<24)yCursor++;
update_cursor(xCursor,yCursor);
  
new_line_term();
}

else if(argc==2&&strcmp(argv[0],"cd",2)==0){
  if(changeDirectory(argv[1])==0){
    backspace();
    printf(" Carpeta no encontrada ");
if(yCursor<24)yCursor++;
update_cursor(xCursor,yCursor);
new_line_term();
  }
}


else{
  backspace();
  printf("Uhh me mataste, no tengo el comando ");
  printf((argv[0]));
  new_line_term();
xCursor=1;
if(yCursor<24)yCursor++;
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
	  if(s==128&&ch!=128){

if(boolOnTest){boolOnTest=0;init_term(mboot);}
    new_line_term();xCursor=1;if(yCursor<24)yCursor++;update_cursor(xCursor,yCursor);
    formatCommand();
    detectCommands();
    }
		if(s==129&&ch!=129&&xCursor>1&&!boolOnTest){

      if(boolOnTest){boolOnTest=0;init_term(mboot);}
      backspace();indexLetter--;line[indexLetter]=' ';xCursor--;update_cursor(xCursor,yCursor);}
    if(s!=ch&&s!=0&&s<128&&!boolOnTest){
	printf("%c",(char)s);line[indexLetter]=s;indexLetter++;xCursor++;update_cursor(xCursor,yCursor);}
		

}

void new_line_term(){
printf("/n>n");
indexLetter=0;
}

