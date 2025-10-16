ENTRY(_start)         /* Define el símbolo de entrada */

SECTIONS {
  . = 0x2000000;     /* Dirección donde querés cargar tu programa */

  .text : {
    *(.text)          /* Todo el código */
  }

  .data : {
    *(.data)
  }

  .bss : {
    *(.bss) 
    *(COMMON)
  }
  PROVIDE(end = .);
}
