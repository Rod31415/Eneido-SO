export PATH=$PATH:/usr/local/i386elfgcc/bin

#nasm "boot/boot.asm" -f bin -o "bin/boot.bin"
nasm "src/kernel.asm" -f elf -o "bin/kernelE.o"
#nasm "boot/zeroes.asm" -f bin -o "bin/zeroes.bin"

nasm "src/realEntry.asm" -f elf -o "bin/realEntry.o"
#nasm "src/disk.asm" -f elf -o "bin/disk.o"

i386-elf-g++ -ffreestanding -m32 -c -g "src/kernel.cpp" -o "bin/kernel.o"
i386-elf-g++ -ffreestanding -m32 -c -g "src/functions.cpp" -o "bin/functions.o"
i386-elf-g++ -ffreestanding -m32 -c -g "src/utilities.cpp" -o "bin/utilities.o"
i386-elf-g++ -ffreestanding -m32 -c -g "src/mem.cpp" -o "bin/mem.o"
i386-elf-g++ -ffreestanding -m32 -c -g "src/term.cpp" -o "bin/term.o"
i386-elf-g++ -ffreestanding -m32 -c -g "src/string.cpp" -o "bin/string.o"
i386-elf-g++ -ffreestanding -m32 -c -g "src/gdt.cpp" -o "bin/gdt.o"
i386-elf-g++ -ffreestanding -m32 -c -g "src/vfs.cpp" -o "bin/vfs.o"
i386-elf-g++ -ffreestanding -m32 -c -g "src/disk.cpp" -o "bin/diskDriver.o"
i386-elf-g++ -ffreestanding -m32 -c -g "src/editor.cpp" -o "bin/editor.o"
i386-elf-g++ -ffreestanding -m32 -c -g "src/games.cpp" -o "bin/games.o"
i386-elf-g++ -ffreestanding -m32 -c -g "src/vga.cpp" -o "bin/vga.o"
i386-elf-g++ -ffreestanding -m32 -c -g "src/rtc.cpp" -o "bin/rtc.o"
i386-elf-g++ -ffreestanding -m32 -c -g "src/math.cpp" -o "bin/math.o"

i386-elf-ld -o "grub/boot/kernel.elf" -T linker.ld -m elf_i386 "bin/math.o" "bin/rtc.o" "bin/vga.o" "bin/games.o" "bin/editor.o" "bin/diskDriver.o" "bin/vfs.o" "bin/realEntry.o" "bin/gdt.o" "bin/string.o" "bin/term.o" "bin/kernelE.o" "bin/kernel.o" "bin/functions.o" "bin/utilities.o" "bin/mem.o" -z noexecstack

grub-mkrescue -o ENEIDO.iso grub/
#sudo cat "bin/boot.bin" "bin/full_kernel.bin" "bin/zeroes.bin" >"bin/OS.bin"

#REMOVE ALL NON USE FILES
rm bin/kernelE.o
rm bin/kernel.o
rm bin/functions.o
rm bin/utilities.o
rm bin/mem.o
rm bin/term.o
rm bin/string.o
rm bin/gdt.o
rm bin/realEntry.o
rm bin/vfs.o
rm bin/disk.o
rm bin/editor.o
rm bin/games.o
rm bin/vga.o
rm bin/rtc.o
rm bin/math.o

qemu-system-i386 "ENEIDO.iso"
