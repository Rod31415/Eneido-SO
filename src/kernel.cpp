#include "headers/kernel.h"

extern "C" void _main(multiboot_info *mboot)
{

	gdt_install();
	idt_install();
  initVFS();
  createFile("Leeme.txt");
 inport(0x3da);
 outport(0x3c0,0x30);
 uint8 res=inport(0x3c1);
 outport(0x3c0,res&0xF7);




vbe_mode_info_struct* vbe=(vbe_mode_info_struct*)(uint32)mboot->vbe_mode_info;

uint8 *fb=(uint8 *)(uint32)vbe->framebuffer;


initVGA(fb);
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
