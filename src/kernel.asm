[bits 32]
section .multiboot
ALIGN 4 
DD 0x1BADB002
DD 0x00000003
DD -(0x1BADB002+0x00000003)

section .text 
global start
extern _main
start:
    mov esp, stack_top

    push ebx
    
    call _main
    cli
_loop:  
    hlt
    jmp _loop


section .bss
stack_bottom:
resb 16384
stack_top:

