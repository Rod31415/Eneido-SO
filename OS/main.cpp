#include "../headers/main.h"

/* 
          # # #
      # #       # #
    #               #
  #        ###        #
  #      ##   ##      #
#       #       #       # 
#       #       #       #
#       #       #       #
  #      #     #      #
  #      #     #      #
    #   #       #   #
      # #       # # 
          
 
 */ 

void drawOpenSource(){
	changeColor(0x02);
	printf("          #####/n");
	printf("      #############/n");
	printf("    #################/n");
	printf("  #####################/n");
	printf("  #########   #########/n");
	printf("#########       #########/n");
	printf("#########   #   #########/n");
	printf("#########       #########/n");
	printf("  ########     ########/n");
	printf("  ########     ########/n");
	printf("    #####       #####/n");
	printf("      ###       ###");

gotoxy(40,6);
printf("ENEIDO SO ");
gotoxy(40,7);
printf(" 32 bits ");
gotoxy(1,14);
uint32 address=0x0f00;
int32 a=*(int32*)address;
printf("Sectores cargados: %d*512 bytes = %d bytes/n/n",a,a*512);
address=0x0f10;
a=*(int32*)address;
printf("Memoria Disponible: %d KB",a);
}

void __main(){
	
	cls(0x0f);
	uint8 character=0;
drawOpenSource();
gotoxy(0,18);
changeColor(0x0f);
printf("-");
char ch=0,s=0;
	while(1){
	for(int i=0;i<1;i++)sleep();
	character=inport(0x60);
	ch=s;
	s=getKeyboardKey(character);
	if(s!=ch&&s!=0){
	//gotoxy(1,18);
	printf("%c",s);}
		if(character==28){printf("/n");}
		if(character==14){backspace();}

	}

}
