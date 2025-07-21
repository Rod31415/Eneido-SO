#include "headers/elf.h"

//#define DEBUG

#define ELF32_R_SYM(info)             ((info)>>8)
#define ELF32_R_TYPE(info)            ((unsigned char)(info))
#define ELF32_R_INFO(sym, type)       (((sym)<<8)+(unsigned char)(type))

void applyRelocationElf(Elf32_Rel *rel,Elf32_Shdr* sections, Elf32_Sym* symtab, char* strtab, uint32 loadbase,uint32 sectionbase,uint32 n);

void ElfLoadObjectFile(uint32 base_address){
    Elf32_Ehdr* s=(Elf32_Ehdr*)base_address;

		Elf32_Shdr* section_headers=(Elf32_Shdr*)(s->e_shoff+base_address);


        Elf32_Sym *symbolTable;
        char *stringTable;
		uint32 num_symbols;

		Elf32_Shdr* textTable;


		for(uint32 i=0;i<s->e_shnum;i++){
			if(section_headers[i].sh_type==0x2){
				symbolTable=(Elf32_Sym*)(section_headers[i].sh_offset+base_address);
				Elf32_Shdr strtabheader=section_headers[section_headers[i].sh_link];
				stringTable=(char *)(strtabheader.sh_offset + base_address);
				num_symbols=section_headers[i].sh_size/section_headers[i].sh_entsize;
			}
			
			uint32 s_off=section_headers[s->e_shstrndx].sh_offset+base_address;
			char* shstrtab=(char*)s_off;
			char* section_name=shstrtab+section_headers[i].sh_name;
			if(strcmp(section_name,".text")==0){
				textTable=&section_headers[i];
			}
		}

		
		uint32 sectionbase=0x500000;

		memcpy((uint32)(base_address+textTable->sh_offset),(uint32)sectionbase,textTable->sh_size);

		char* symbolStrings=(char*)(section_headers[s->e_shstrndx].sh_offset+base_address);

        
        for(uint32 i=0;i<s->e_shnum;i++){
			if(section_headers[i].sh_type==0x9&&strcmp(&symbolStrings[section_headers[i].sh_name],".rel.text")==0){
				#ifdef DEBUG
				printf("Relocation on :");
				printf(&symbolStrings[section_headers[i].sh_name]);
				printf("/n");
				#endif
				Elf32_Rel* rels=(Elf32_Rel*)(section_headers[i].sh_offset+base_address);

				int num_rels=section_headers[i].sh_size/sizeof(Elf32_Rel);

				uint32 target= sectionbase;

				for(uint32 j=0;j< num_rels;j++){
					applyRelocationElf(&rels[j],section_headers,symbolTable,stringTable,base_address,target,j);
					//printf("/n");
				}

			}
			#ifdef DEBUG
				printf("-- %d --/n",section_headers[i].sh_type);
			#endif
		}

		

		/*for(uint32 i=0;i<num_symbols;i++){
			Elf32_Sym* sm=&symbolTable[i];
			char* name=stringTable+sm->st_name;
			if(strcmp(name,"main")==0){*/
				void* addr=(void*)(sectionbase);
				typedef int(*func_t)(void);

				func_t call=(func_t)addr;

				//__asm__ __volatile__("movl $0x7c00, %esp");
				uint32 a=call();

				//printf("Result : %d/n",a);
				uint8* code=(uint8*)call;
				#ifdef DEBUG
				for(uint32 i=0;i< textTable->sh_size;i++){
				printf("%x/",code[i]);}
				#endif
		/*	}
		}*/

//#ifdef DEBUG
		printf("Program Ended -  Returned : %d",a);
//		#endif
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
	if (!isKeyPressed())
		continue;
	eatKeyBuffered();
	if (isNumeric(ch))
	{
		printf("%c", ch );
		num[Intindex++] = ch;
		refresh();
	}
	if (ch == ENTER)
		break;
	}

	return toInt(num);
}



struct Function exportedFunction[]={
	{"_Z6printfPciiiii",(void*)printf},
	{"_Z5scanfv",(void*)ret}
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