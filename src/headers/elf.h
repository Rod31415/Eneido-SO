#ifndef ELF_H
#define ELF_H 
#include "types.h"
#include "functions.h"
#include "string.h"

#define EI_NIDENT 16

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


void ElfLoadObjectFile(uint32 base_address);

#endif