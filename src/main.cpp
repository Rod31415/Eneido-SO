#include "headers/main.h"

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

/*void drawOpenSource(){
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
*/
void __main(multiboot_info *mboot){
	/*cls(0x0f);
	uint8 character=0;
drawOpenSource();
gotoxy(0,18);
changeColor(0x0f);
printf("-");
uint8 ch=0,s=0;
//showMem(0xb8000,0xb8100);
while(1){
	for(int i=0;i<1;i++)sleep();
	character=inport(0x60);
	ch=s;
	s=getKeyboardKey(character);
	if(s==128&&ch!=128){printf("/n");}
		if(s==129&&ch!=129){backspace();}
  if(s!=ch&&s!=0&&s<128){	
	printf("%c",(char)s);}
		

	}*/
  init_term(mboot);
  while(1){

    loop_term();
  }
}
