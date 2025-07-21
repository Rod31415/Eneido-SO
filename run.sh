export PATH=$PATH:/usr/local/i386elfgcc/bin

#nasm "boot/boot.asm" -f bin -o "bin/boot.bin"
nasm "src/kernel.asm" -f elf -o "bin/kernelE.o"
#nasm "boot/zeroes.asm" -f bin -o "bin/zeroes.bin"

nasm "src/realEntry.asm" -f elf -o "bin/realEntry.o"

nasm "src/isr.asm" -f elf -o "bin/isr.o"
#nasm "src/disk.asm" -f elf -o "bin/disk.o"

#/usr/local/i386elfgcc/bin/i386-elf-g++ -ffreestanding -Wno-write-strings -m32 -c -g "test.cpp" -o "test.o"
#/usr/local/i386elfgcc/bin/i386-elf-ld -o "grub/boot/test.elf" -T linker.ld -m elf_i386 "test.o" -z noexecstack

i386-elf-g++ -ffreestanding -Wno-write-strings -m32 -c -g "src/kernel.cpp" -o "bin/kernel.o"
i386-elf-g++ -ffreestanding -Wno-write-strings -m32 -c -g "src/functions.cpp" -o "bin/functions.o"
i386-elf-g++ -ffreestanding -Wno-write-strings -m32 -c -g "src/utilities.cpp" -o "bin/utilities.o"
i386-elf-g++ -ffreestanding -Wno-write-strings -m32 -c -g "src/mem.cpp" -o "bin/mem.o"
i386-elf-g++ -ffreestanding -Wno-write-strings -m32 -c -g "src/term.cpp" -o "bin/term.o"
i386-elf-g++ -ffreestanding -Wno-write-strings -m32 -c -g "src/string.cpp" -o "bin/string.o"
i386-elf-g++ -ffreestanding -Wno-write-strings -m32 -c -g "src/gdt.cpp" -o "bin/gdt.o"
i386-elf-g++ -ffreestanding -Wno-write-strings -m32 -c -g "src/idt.cpp" -o "bin/idt.o"
i386-elf-g++ -ffreestanding -Wno-write-strings -m32 -c -g "src/vfs.cpp" -o "bin/vfs.o"
i386-elf-g++ -ffreestanding -Wno-write-strings -m32 -c -g "src/disk.cpp" -o "bin/disk.o"
i386-elf-g++ -ffreestanding -Wno-write-strings -m32 -c -g "src/editor.cpp" -o "bin/editor.o"
i386-elf-g++ -ffreestanding -Wno-write-strings -m32 -c -g "src/games.cpp" -o "bin/games.o"
i386-elf-g++ -ffreestanding -Wno-write-strings -m32 -c -g "src/vga.cpp" -o "bin/vga.o"
i386-elf-g++ -ffreestanding -Wno-write-strings -m32 -c -g "src/rtc.cpp" -o "bin/rtc.o"
i386-elf-g++ -ffreestanding -Wno-write-strings -m32 -c -g "src/math.cpp" -o "bin/math.o"
i386-elf-g++ -ffreestanding -Wno-write-strings -m32 -c -g "src/pci.cpp" -o "bin/pci.o"
i386-elf-g++ -ffreestanding -Wno-write-strings -m32 -c -g "src/interpreter.cpp" -o "bin/interpreter.o"
i386-elf-g++ -ffreestanding -Wno-write-strings -m32 -c -g "src/fat.cpp" -o "bin/fat.o"
i386-elf-g++ -ffreestanding -Wno-write-strings -m32 -c -g "src/nics.cpp" -o "bin/nics.o"
i386-elf-g++ -ffreestanding -Wno-write-strings -m32 -c -g "src/ne2000.cpp" -o "bin/ne2000.o"
i386-elf-g++ -ffreestanding -Wno-write-strings -m32 -c -g "src/rtl8139.cpp" -o "bin/rtl8139.o"
i386-elf-g++ -ffreestanding -Wno-write-strings -m32 -c -g "src/keyboard.cpp" -o "bin/keyboard.o"
i386-elf-g++ -ffreestanding -Wno-write-strings -m32 -c -g "src/mouse.cpp" -o "bin/mouse.o"
i386-elf-g++ -ffreestanding -Wno-write-strings -m32 -c -g "src/elf.cpp" -o "bin/elf.o"

i386-elf-ld -o "grub/boot/kernel.elf" -T linker.ld -m elf_i386 "bin/elf.o" "bin/mouse.o" "bin/keyboard.o" "bin/rtl8139.o" "bin/ne2000.o" "bin/nics.o" "bin/fat.o" "bin/interpreter.o" "bin/pci.o" "bin/isr.o" "bin/idt.o" "bin/math.o" "bin/rtc.o" "bin/vga.o" "bin/games.o" "bin/editor.o" "bin/disk.o" "bin/vfs.o" "bin/realEntry.o" "bin/gdt.o" "bin/string.o" "bin/term.o" "bin/kernelE.o" "bin/kernel.o" "bin/functions.o" "bin/utilities.o" "bin/mem.o" -z noexecstack

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
rm bin/idt.o
rm bin/isr.o
rm bin/pci.o
rm bin/fat.o
rm bin/interpreter.o
rm bin/ne2000.o
rm bin/nics.o
rm bin/keyboard.o
rm bin/rtl8139.o


sudo ip tuntap add dev tap0 mode tap
sudo ip link set tap0 up


#qemu-system-i386 -net nic,model=rtl8139 "ENEIDO.iso" -netdev tap,id=net0,ifname=tap0,script=no,downscript=no
qemu-system-i386 -hda "ENEIDO.iso" \
  -device rtl8139,netdev=n1 \
  -netdev tap,id=n1,ifname=tap0,script=no,downscript=no #\
#-object filter-dump,id=dump,netdev=net0,file=out.pcap

#wireshark out.pcap
#sudo tcpdump -i tap0
