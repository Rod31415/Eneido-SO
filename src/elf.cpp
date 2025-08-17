#include "headers/elf.h"


uint32 kernel_esp;
uint32 kernel_ebp;

#define ELF32_R_SYM(info)             ((info)>>8)
#define ELF32_R_TYPE(info)            ((unsigned char)(info))
#define ELF32_R_INFO(sym, type)       (((sym)<<8)+(unsigned char)(type))

static inline uint32 read_esp(){uint32 v; asm volatile("movl %%esp,%0":"=r"(v)); return v;}
static inline uint32 read_ebp(){uint32 v; asm volatile("movl %%ebp,%0":"=r"(v)); return v;}

void applyRelocationElf(Elf32_Rel *rel,Elf32_Shdr* sections, Elf32_Sym* symtab, char* strtab, uint32 loadbase,uint32 sectionbase,uint32 n);

void ElfLoadObjectFile(uint32 base_address, int argc, char argv[10][80]){
    Elf32_Ehdr* s=(Elf32_Ehdr*)base_address;

		Elf32_Shdr* section_headers=(Elf32_Shdr*)(s->e_shoff+base_address);

		#define PT_LOAD 1
				Elf32_Phdr *program_headers=(Elf32_Phdr*)(s->e_phoff+base_address);
				for(uint32 i=0;i<s->e_phnum;i++){
					
					if(program_headers[i].p_type==PT_LOAD){
						uint32 memaddr=(program_headers[i].p_vaddr+(program_headers[i].p_align-1))&~(program_headers[i].p_align-1);
						((uint8*)memaddr)[0]=23;
						memcpy(program_headers[i].p_offset+base_address,memaddr,program_headers[i].p_filesz);
						memset((program_headers[i].p_vaddr+program_headers[i].p_filesz),0,program_headers[i].p_memsz - program_headers[i].p_filesz);
						//	printf("%h   %d/n ",memaddr,program_headers[i].p_memsz);
					}
				}
				
				uint32 newbase_address=s->e_entry;
				
				uint32 stack[128];
				
				stack[0]=argc;

				uint32 i;
				for(i=0;i<argc;i++){
					stack[i+1]=(uint32)argv[i];
				}
				stack[i+2]=0;
				stack[i+3]=0;

				uint32 *stack_ptr =&stack[0];


				kernel_esp=read_esp();
				kernel_ebp=read_ebp();

				asm volatile(
					"movl %0,%%esp\n"
					"jmp *%1\n"
					:
					: "r"(stack_ptr), "r"(newbase_address)
					: "memory"
				);
				
				printf("Return");

				#ifdef DEBUG
				for(uint32 i=0;i< 80;i++){
				printf("%x/",code[i]);}
				#endif


#ifdef DEBUG
		printf("Program Ended -  Returned : %d",0);
		#endif
}

#define R_386_NONE 0
#define R_386_32 1
#define R_386_PC32 2

uint32 resolveSymbol(char *symbol);

void applyRelocationElf(Elf32_Rel *rel, Elf32_Shdr *sections, Elf32_Sym* symtab, char* strtab, uint32 loadbase,uint32 sectionbase,uint32 n){
	
	int symIdx=ELF32_R_SYM(rel->r_info);
	int type=ELF32_R_TYPE(rel->r_info);

	uint32 symbolValue=0;
        //printf("%d ",symIdx);
	if(symIdx){

	Elf32_Sym* sym=&symtab[symIdx];

	#ifdef DEBUG
				char* sym_name=strtab+sym->st_name;
				printf("%d : ",symIdx);
				uint32 index=0;
				while(sym_name[index]!=0){
					printChr(sym_name[index++]);
				}
				
				printf("-");
	#endif

	
	if(sym->st_shndx==0){
		char* sym_name=strtab+sym->st_name;
		symbolValue=resolveSymbol(sym_name);
	}
	else{
		Elf32_Shdr tr=sections[sym->st_shndx];
		uint32 sect=loadbase+tr.sh_offset;
		symbolValue=sect+sym->st_value;

		#ifdef DEBUG
		printf("New entry: ");
		printf((char*)symbolValue);
		printf("/n");
		#endif

	}
                        
	uint32* reloc_addr=(uint32*)(sectionbase+rel->r_offset);

	uint32 addend=*reloc_addr;

	switch(type){
		case R_386_32:
			*reloc_addr=symbolValue+addend;
			break;
		case R_386_PC32:
			*reloc_addr=symbolValue+addend-(uint32)reloc_addr;
			break;

	}


    }
	
}

struct Function{

char *symbol;
void *pointer;

};

#define ENTER 128
int ret(){
	uint8 ch;
	uint32 Intindex = 0;
	int8 num[32];
	eatKeyBuffered();
	while (1)
	{
	ch = getLastAsciiKey();
	if (isKeyBuffered()){
	        
	        if (isNumeric(ch))
	        {
				eatKeyBuffered();
	        	printf("%c", ch );
	        	num[Intindex++] = ch;
	        	refresh();
	        }
	        if (ch == ENTER){
			eatKeyBuffered();
	        	break;}
	        }
        }
		printf("/n");
	return toInt(num);
	
}

void newsrand(){
srand(time());
}

void newsleep(){
	sleep(10);
}

struct Function exportedFunction[]={
	{"_Z6printfPciiiii",(void*)printf},
	{"_Z5scanfv",(void*)ret},
	{"_Z4randv",(void*)rand},
	{"_Z5srandv",(void*)newsrand},
	{"_Z5sleepv",(void*)newsleep}
};



uint32 resolveSymbol(char *symbol){

Function res;
uint32 max=sizeof(exportedFunction)/sizeof(Function);
for(uint32 i=0;i<max;i++){
	if(strcmp(exportedFunction[i].symbol,symbol)==0){
		return (uint32)exportedFunction[i].pointer;
	}
}
return 0;
}