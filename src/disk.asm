[BITS 16]
section .text
global readDisk
readDisk:

  mov word [dap + 2], 1
  mov word [dap + 4], buffer
  mov word [dap + 6], ds
  mov dword [dap + 8], 0
  mov dword [dap + 12], 0
  
  mov si, dap
  mov dl, 0x80
  mov ah, 0x42
  int 0x13

ret

  dap:
  db 0x10
  db 0x0
  dw 0
  dw 0
  dw 0
  dq 0



buffer equ 0x7e00
