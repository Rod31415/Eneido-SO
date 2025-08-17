i386-elf-gcc -B/usr/local/i386elfgcc/bin -static -nostdlib \
  -I/opt/eneidoso/musl/include \
  -L/opt/eneidoso/musl/lib \
  -o "../grub/boot/$1.elf" $1.c syscalls_stub.c\
  /opt/eneidoso/musl/lib/crt1.o -lc -lgcc -T linker.ld

  