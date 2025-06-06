#include "headers/main.h"
#include "headers/kernel.h"


extern "C" void _main(multiboot_info *mboot){

printf("Loading kernel");
for(int8 i=0;i<10;i++){
printf(".");sleep();}
//printf("  %d   ",mboot->mem_lower);


__main(mboot);
	return;
}
