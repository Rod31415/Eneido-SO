i386-elf-gcc -B/usr/local/i386elfgcc/bin -static -nostdlib \
  -I/opt/eneidoso/musl/include \
  -L/opt/eneidoso/musl/lib \
  -o "../grub/boot/$1.elf" $1.c syscalls_stub.c /opt/eneidoso/musl/lib/crt1.o -lc -lgcc -T linker.ld

## i386-elf-gcc -B/usr/local/i386elfgcc/bin -static -nostdlib \
# -I"/media/rodrigo/NUEVO VOL/ENEIDO_SA/coreutils/coreutils/coreutils-9.5/lib/" -I/opt/eneidoso/musl/include \
# -o "../../cat.elf" cat.c \
# /opt/eneidoso/musl/lib/crt1.o -lc -lgcc -T linker.ld
