#include "headers/kernel.h"

extern "C" void _main(multiboot_info *mboot)
{

	gdt_install();
	idt_install();
  initVFS();
  createFile("Leeme.txt");
  uint8 buf[512]="     VAR x=1| PRINT('Ingrese su edad : ')| INPUT(x)| PRINT('/n Su edad es :')| PRINT(x)";
  for(uint32 i=0;i<512;i++){
    if(buf[i]=='|'){
      buf[i]=13;
    }
   }
  *(buf)=(uint32)10;
  writeCluster(buf,64*8);
// inport(0x3da);
// outport(0x3c0,0x30);
// uint8 res=inport(0x3c1);
// outport(0x3c0,res&0xF7);

outport(0x60,0xf4 );


vbe_mode_info_struct* vbe=(vbe_mode_info_struct*)(uint32)mboot->vbe_mode_info;

uint8 *fb=(uint8 *)(uint32)vbe->framebuffer;
uint32 width=(uint32)vbe->width;
uint32 height=(uint32)vbe->height;


initVGA(fb,width,height);
clear_screen(0);
  changeColor(0x0f);

initDisk();
  init_term(mboot);


  while (1)
  {

    loop_term();
  }

	return;
}
