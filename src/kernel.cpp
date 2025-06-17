#include "headers/kernel.h"

extern "C" void _main(multiboot_info *mboot)
{
	gdt_install();
	initVFS();
  createFile("Leeme.txt");
 inport(0x3da);
 outport(0x3c0,0x30);
 uint8 res=inport(0x3c1);
 outport(0x3c0,res&0xF7);

initVGA();
clear_screen(0);


  init_term(mboot);


  while (1)
  {

    loop_term();
  }

	return;
}
