[bits 32]
[extern _main]
call _main
jmp $
times 512-($-$$) db 0
