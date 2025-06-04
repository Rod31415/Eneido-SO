[bits 32]

section .multiboot
  ALIGN 4
  DD 0x1BADB002
  DD 0x00000000
  DD -(0x1BADB002+0x00000000)
section .text
  global start
[extern _main]

start:
  cli
  mov esp,stack_space
call _main
hlt

HaltKernel:
cli
hlt
jmp HaltKernel

section .bss
RESB 8192
stack_space:

