#include "headers/kernel.h"

extern "C" void _main(multiboot_info *mboot)
{

	gdt_install();
	idt_install();
  initVFS();
  createFile("Leeme.txt");
uint8 buf[512]="     var x=1 | var s=1 | print('Ingrese su edad: ') input(x) |print('/n') |if(x== 18)|{print('ES MAYOR')|} ";
  for(uint32 i=0;i<512;i++){
    if(buf[i]=='|'){
      buf[i]=13;
    }
   }
  *(buf)=(uint32)10;

  writeCluster(buf,64*8);
outport(0x60,0xf4);



vbe_mode_info_struct* vbe=(vbe_mode_info_struct*)(uint32)mboot->vbe_mode_info;

uint8 *fb=(uint8 *)(uint32)vbe->framebuffer;
uint32 width=(uint32)vbe->width;
uint32 height=(uint32)vbe->height;

initVGA(fb,width,height);
clear_screen(0);
  changeColor(0x0f);

initDisk();

  init_term(mboot);
  PCIInitDrivers();
  PCIShowDevices();
  
  

  
refresh();
  while (1)
  {

   // loop_term();
  }

	return;
}
