export PATH=$PATH:/usr/local/i386elfgcc/bin

nasm "boot/boot.asm" -f bin -o "bin/boot.bin"
nasm "kernel/kernel.asm" -f elf -o "bin/kernelE.o"
nasm "boot/zeroes.asm" -f elf -o "bin/zeroes.bin"
#i386-elf-gcc -o "bin/kernel.o" -m32 -c -g "kernel.cpp" "functions.cpp" "utilities.cpp" "main.cpp" "kernel.cpp" 
gcc -ffreestanding -m32 -c -g "kernel/kernel.cpp" -o "bin/kernel.o"
gcc -ffreestanding -m32 -c -g "function/functions.cpp" -o "bin/functions.o"
gcc -ffreestanding -m32 -c -g "function/utilities.cpp" -o "bin/utilities.o"
#i386-elf-gcc -ffreestanding -m32 -c -g "function/subfunctions.cpp" -o "bin/subfunctions.o"
gcc -ffreestanding -m32 -c -g "OS/main.cpp" -o "bin/main.o"

gcc -o "bin/full_kernel.bin" -Ttext 0x1000 "bin/kernelE.o" "bin/kernel.o" "bin/functions.o" "bin/utilities.o" "bin/main.o" --oformat binary
sudo cat "bin/boot.bin" "bin/full_kernel.bin" > "bin/every.bin"
sudo cat "bin/every.bin" "bin/zeroes.bin" > "bin/OS.bin"
#qemu-system-i386 "bin/OS.bin"
qemu-system-i386 -drive format=raw,file="bin/OS.bin"



