[bits 16]
[org 0x7c00]                        
KERNEL_LOCATION equ 0x7e00
SECTOR_LOCATION equ 0x0f00
MEMORY_LOCATION equ 0x0f10


mov [BOOT_DISK], dl                 



xor ax, ax      
mov ds, ax
mov es, ax
cli
mov bp, 0x8000
mov sp, bp
sti

mov ah,0x42
mov dl,[BOOT_DISK]
mov si, DPACK

;mov bx, KERNEL_LOCATION
;mov dh, 0x00
;mov dl, [BOOT_DISK]
;mov cl, 0x02
;mov ch, 0x00
;mov ah, 0x02
;mov al, 20
int 0x13     
jc disk_error
mov [SECTOR_LOCATION],al                       
mov ah, 0x0
mov al, 0x3
int 0x10     

int 0x12
mov [MEMORY_LOCATION],ax

mov si,message
call print

jmp toPM

disk_error:
jmp $

CODE_SEG equ 0x8
DATA_SEG equ 0x10

toPM:
cli
lgdt [GDT_descriptor]
mov eax, cr0
or eax, 1
mov cr0, eax
jmp 0x08:start_pm

jmp $
                                    
BOOT_DISK: db 0

print:
mov al, [si]
printloop:
cmp al,0
je done
mov al, [si]
mov ah, 0eh
int 0x10
inc si
jmp printloop
done:
ret


DPACK:
db 10h
db 0
dw 20
dw 0x7e00
dw 0
dd 1
dd 0


GDT_start:
    GDT_null:
        dd 0x0
        dd 0x0

    GDT_code:
        dw 0xffff
        dw 0x0
        db 0x0
        db 0b10011010
        db 0b11001111
        db 0x0

    GDT_data:
        dw 0xffff
        dw 0x0
        db 0x0
        db 0b10010010
        db 0b11001111
        db 0x0

GDT_end:

GDT_descriptor:
    dw GDT_end - GDT_start - 1
    dd GDT_start


[bits 32]
start_pm:
    mov ax, DATA_SEG
	mov ds, ax
	mov ss, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	
	mov ebp, 0x90000		; 32 bit stack base pointer
	mov esp, ebp
	
    jmp 0x08:KERNEL_LOCATION



	

message: db "Holaaa",0
                                     
 
times 510-($-$$) db 0              
dw 0xaa55
