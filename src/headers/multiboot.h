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


struct vbe_mode_info_struct{
  uint16 attributes;
  uint8 window_a, window_b;
  uint16 granularity;
  uint16 window_size;
  uint16 segment_a, segment_b;
  uint32 win_func_ptr;
  uint16 pitch;
  uint16 width, height;
  uint8 w_char,y_char, planes, bpp, banks;
  uint8 memory_model, bank_size, image_pages;
  uint8 reserved0;

  uint8 red_mask,red_position;
  uint8 green_mask,green_position;
  uint8 blue_mask,blue_position;
  uint8 reserved_mask,reserved_position;
  uint8 direct_color_attributes;

  uint32 framebuffer;
  uint32 off_screen_mem_off;
  uint16 off_screen_mem_size;
}__attribute__((packed));

#endif 
