#ifndef MULTIBOOT_H
#define MULTIBOOT_H
#include "types.h"
typedef struct multiboot_info{
    uint32 flags;            // Flags indicating which fields are valid
    uint32 mem_lower;        // Amount of lower memory (below 1 MB) in KB
    uint32 mem_upper;        // Amount of upper memory (above 1 MB) in KB
    uint32 boot_device;      // Boot device (BIOS disk device)
    uint32 cmdline;          // Pointer to the command line string
    uint32 mods_count;       // Number of modules loaded
    uint32 mods_addr;        // Pointer to the first module structure
    uint32 syms[4];          // Symbol table information (deprecated)
    uint32 mmap_length;      // Size of the memory map
    uint32 mmap_addr;        // Pointer to the memory map
    uint32 drives_length;    // Size of the BIOS drive information
    uint32 drives_addr;      // Pointer to the BIOS drive information
    uint32 config_table;     // Pointer to the ROM configuration table
    uint32 boot_loader_name; // Pointer to the bootloader name string
    uint32 apm_table;        // Pointer to the APM (Advanced Power Management) table
    uint32 vbe_control_info; // VBE (VESA BIOS Extensions) control information
    uint32 vbe_mode_info;    // VBE mode information
    uint16 vbe_mode;         // VBE mode number
    uint16 vbe_interface_seg; // VBE interface segment
    uint16 vbe_interface_off; // VBE interface offset
    uint16 vbe_interface_len; // VBE interface length 
}__attribute__((packed))multiboot_info_t;

#endif 
