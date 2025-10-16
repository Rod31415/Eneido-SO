#include "headers/paging.h"

#define PAGE_SIZE 4096
#define NUM_ENTRIES 1024


#define KERNELBASE 0x100000
#define KERNELSIZE 59*PAGE_SIZE

static uint32 page_directory[NUM_ENTRIES] __attribute__((aligned(PAGE_SIZE)));
static uint32 f_page_table[NUM_ENTRIES] __attribute__((aligned(PAGE_SIZE)));


void mapPage(uint32 *page_directory, void *virt_addr, uint32 phys_addr, uint32 flags) {
    uint32 va = (uint32)virt_addr;
    uint32 pd_index = (va >> 22) & 0x3FF;
    uint32 pt_index = (va >> 12) & 0x3FF;

    uint32 *page_table = (uint32*)(page_directory[pd_index] & 0xFFFFF000);

    page_table[pt_index] = (phys_addr & 0xFFFFF000) | (flags & 0xFFF) | 0x1;

    asm volatile("invlpg (%0)" :: "r"(virt_addr) : "memory");
}


void* allocMapPage(uint32 *page_directory, void *virt_addr, uint32 flags){
  void *phys_addr=palloc(PAGE_SIZE);
  if(!phys_addr){
    return 0;
  }
  mapPage(page_directory,virt_addr,(uint32)phys_addr,flags);


  return phys_addr; 
}

void loadPageDirectory(uint32 *pd){
asm volatile("mov %0, %%cr3" :: "r"(pd));
}

void enablePaging(){
asm volatile(
    "mov %cr0,%eax\n"
    "or $0x80000000,%eax\n"
    "mov %eax,%cr0\n"
    );


}

void setupPaging(){
  
initPAlloc();

  page_directory[0]=((uint32)f_page_table)|3;


for(uint32 addr=KERNELBASE;addr<(KERNELBASE+KERNELSIZE);addr+=PAGE_SIZE){
mapPage(page_directory, (void*)addr, addr, 0x3);
}


loadPageDirectory(page_directory);
enablePaging();
}
