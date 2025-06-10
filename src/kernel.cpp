#include "headers/main.h"
#include "headers/kernel.h"



extern "C" void _main(multiboot_info *mboot){

gdt_install();

initVFS();

__main(mboot);
	return;
}
