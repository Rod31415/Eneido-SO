export PATH=$PATH:/usr/local/i386elfgcc/bin

#nasm "boot/boot.asm" -f bin -o "bin/boot.bin"
nasm "src/kernel.asm" -f elf -o "bin/kernelE.o"
#nasm "boot/zeroes.asm" -f bin -o "bin/zeroes.bin"

i386-elf-g++ -ffreestanding -m32 -c -g "src/kernel.cpp" -o "bin/kernel.o"
i386-elf-g++ -ffreestanding -m32 -c -g "src/functions.cpp" -o "bin/functions.o"
i386-elf-g++ -ffreestanding -m32 -c -g "src/utilities.cpp" -o "bin/utilities.o"
i386-elf-g++ -ffreestanding -m32 -c -g "src/main.cpp" -o "bin/main.o"

#g++ -m32 -c -g "kernel/kernel.cpp" -o "bin/kernel.o"
#g++ -m32 -c -g "function/functions.cpp" -o "bin/functions.o"
#g++ -m32 -c -g "function/utilities.cpp" -o "bin/utilities.o"
#g++ -m32 -c -g "OS/main.cpp" -o "bin/main.o"

#gcc -o "bin/full_kernel.bin" -Ttext=0x1000 "bin/kernelE.o" "bin/kernel.o" "bin/functions.o" "bin/utilities.o" "bin/main.o" -Wl,--oformat=binary -z noexecstack

i386-elf-ld -o "grub/boot/kernel.elf" -T linker.ld -m elf_i386 "bin/kernelE.o" "bin/kernel.o" "bin/functions.o" "bin/utilities.o" "bin/main.o" -z noexecstack

grub-mkrescue -o ENEIDO.iso grub/
#sudo cat "bin/boot.bin" "bin/full_kernel.bin" "bin/zeroes.bin" >"bin/OS.bin"

#REMOVE ALL NON USE FILES
rm bin/kernelE.o
rm bin/kernel.o
rm bin/functions.o
rm bin/utilities.o
rm bin/main.o

qemu-system-i386 "ENEIDO.iso"
#-drive format=raw,file="bin/OS.bin"
