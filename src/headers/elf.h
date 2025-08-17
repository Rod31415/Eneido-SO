#ifndef ELF_H
#define ELF_H 
#include "types.h"
#include "functions.h"
#include "string.h"
#include "math.h"
#include "rtc.h"
#include "pit.h"
#include "ctx.h"

#define EI_NIDENT 16

extern void *kernel_eip;
extern uint32 kernel_esp,kernel_ebp;

typedef struct Elf32_Ehdr{
        unsigned char   e_ident[EI_NIDENT]; 
        uint16      e_type;
        uint16      e_machine;
        uint32      e_version;
        uint32      e_entry;
        uint32      e_phoff;
        uint32      e_shoff;
        uint32      e_flags;
        uint16      e_ehsize;
        uint16      e_phentsize;
        uint16      e_phnum;
        uint16      e_shentsize;
        uint16      e_shnum;
        uint16      e_shstrndx;
}__attribute__((packed))Elf32_Ehdr_t;

typedef struct Elf32_Shdr{
	uint32	sh_name;
	uint32	sh_type;
	uint32	sh_flags;
	uint32	sh_addr;
	uint32	sh_offset;
	uint32	sh_size;
	uint32	sh_link;
	uint32	sh_info;
	uint32	sh_addralign;
	uint32	sh_entsize;
}__attribute__((packed))Elf32_Shdr_t;


typedef struct Elf32_Phdr{
uint32 p_type;
uint32 p_offset;
uint32 p_vaddr;
uint32 p_paddr;
uint32 p_filesz;
uint32 p_memsz;
uint32 p_flags;
uint32 p_align;
}__attribute__((packed))Elf32_Phdr_t;

typedef struct {
	uint32	st_name;
	uint32	st_value;
	uint32	st_size;
	unsigned char	st_info;
	unsigned char	st_other;
	uint16	st_shndx;
} Elf32_Sym;

typedef struct {
        uint32      r_offset;
        uint32      r_info;
} Elf32_Rel;


void ElfLoadObjectFile(uint32 base_address, int argc, char argv[10][80]);

#endif