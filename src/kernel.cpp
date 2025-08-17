#include "headers/kernel.h"

 void * kernel_eip;

extern "C" void _main(multiboot_info *mboot)
{

	gdt_install();
	idt_install();
	PCIInitDrivers();

vbe_mode_info_struct *vbe = (vbe_mode_info_struct *)(uint32)mboot->vbe_mode_info;

	uint32 *fb = (uint32 *)(uint32)vbe->framebuffer;
	uint32 width = (uint32)vbe->width;
	uint32 height = (uint32)vbe->height;

	initVGA(fb, width, height);
	changeColor(COLOR_WHITE);


	initDisk();

	initAlloc();

	/*uint32 ptr1=(uint32)kalloc(10);
	uint32 ptr2=(uint32)kalloc(10);

	printf("PRIMER  %d/n",ptr1);
	printf("SEGUNDO %d/n",ptr2);

	kfree((void*)ptr1);

	ptr1=(uint32)kalloc(100);

	printf("PRIMER  %d/n",ptr1);
	printf("SEGUNDO %d/n",ptr2);*/

	/*refresh();
	while(1){}*/
	//initMouse();
	initKeyboard();
	

	initRootDirectoryWithModules(mboot);

	kernel_eip=&&kernel_entry;
  init_term(mboot);

  kernel_entry:
	while (1)
	{
		 loop_term();
	}

	return;
}
