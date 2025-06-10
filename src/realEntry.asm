%if 0
[BITS 32]
section .text
;;gpt_pointer: dq 0
saveebp: dw 0
saveesp: dw 0

global switch_to_real_mode


switch_to_real_mode:
mov [saveebp],ebp
mov [saveesp],esp
mov eax, [esp+4]
mov dword[gpt_pointer],eax
  cli
  mov eax,cr0
  and eax, 0xFFFFFFFE
  mov cr0,eax
   jmp 0x0000:real_mode_entry

[BITS 16]

real_mode_entry:

or ax, ax
mov ds, ax
mov es, ax
mov bp, 0x8000
mov sp, bp
sti

;switch_to_protected_mode:
;  cli
;  lgdt [gpt_pointer]
  mov eax, cr0
  or eax, 1
  mov cr0, eax

;[BITS 32]
;db 0xEA
;dd protected_entry
;dw 0x08

;protected_entry:
mov ax, 0x10
mov ds, ax
mov es, ax 
mov ss, ax 
mov fs, ax 
mov gs, ax 
mov ebp, [saveebp]
mov esp, [saveesp]
ret 
%endif
global gdt_flush     
;section .text
gdt_flush:
mov eax, [esp+4]

    lgdt [eax]   
    mov ax, 0x10    
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    jmp 0x08:flush2  
flush2:
    ret 


