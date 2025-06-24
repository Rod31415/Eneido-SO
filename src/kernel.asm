[bits 32]
section .multiboot
ALIGN 4 
DD 0x1BADB002
DD 0x00000007
DD -(0x1BADB002+0x00000007)

DD 0
DD 0
DD 0
DD 0
DD 0

DD 0x0
DD 640
DD 480
DD 8

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

