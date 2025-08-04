#include "headers/kernel.h"



extern "C" void _main(multiboot_info *mboot)
{

	gdt_install();
	idt_install();
	

vbe_mode_info_struct *vbe = (vbe_mode_info_struct *)(uint32)mboot->vbe_mode_info;

	uint32 *fb = (uint32 *)(uint32)vbe->framebuffer;
	uint32 width = (uint32)vbe->width;
	uint32 height = (uint32)vbe->height;

	initVGA(fb, width, height);
	changeColor(COLOR_WHITE);


	initDisk();


	PCIInitDrivers();
	
	//initMouse();
	initKeyboard();

	initRootDirectory();
	FILE* f=fopen("LEEME   TXT");
	
	fputs("HOLA",f);
	

  init_term(mboot);
	while (1)
	{
		 loop_term();
	}

	return;
}
