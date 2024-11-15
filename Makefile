O_FILES = kernel-entry.o kernel.o console/console.o device/portmap/portmap.o

all: qemu_launch

qemu_launch: os.bin
	qemu-system-i386 -drive format=raw,file=os.bin

os.bin: boot.bin kernel.bin
	cat $^ > $@

boot.bin: boot.asm
	nasm $< -f bin -o $@

kernel.bin: ${O_FILES}
	ld -m elf_i386 -o $@ -Ttext 0x1000 ${O_FILES} --oformat binary -nostdlib -static

kernel-entry.o: kernel-entry.elf
	nasm $< -f elf32 -o $@

%.o: %.c
	gcc -m32 -ffreestanding -fno-pie -Iinclude -c ${@:.o=.c} -o $@


device/portmap/portmap.o: device/portmap/portmap.c
	gcc -m32 -ffreestanding -fno-pie -Iinclude -c $< -o $@

clean:
	rm -f *.bin *.o
	find . -name \*.o | xargs --no-run-if-empty rm

