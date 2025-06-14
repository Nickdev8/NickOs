ISO_NAME = NickOS.iso
KERNEL_ELF = build/kernel.elf
OBJS = build/boot.o build/kernel.o build/vga.o build/input.o build/shell.o build/util.o build/ascii_art.o

all: $(ISO_NAME)

build:
	mkdir -p build

build/kernel.elf: $(OBJS)
	x86_64-elf-ld -m elf_i386 -T link.ld -o $@ $(OBJS)

build/boot.o: src/boot.asm | build
	nasm -f elf32 $< -o $@

build/kernel.o: src/kernel.c | build
	x86_64-elf-gcc -m32 -ffreestanding -O2 -Wall -Wextra -c $< -o $@

build/%.o: src/%.c | build
	x86_64-elf-gcc -m32 -ffreestanding -O2 -Wall -Wextra -c $< -o $@


$(KERNEL_ELF): $(OBJS)
	x86_64-elf-ld -m elf_i386 -T link.ld -o $@ $(OBJS)

iso/boot/grub/grub.cfg:
	mkdir -p iso/boot/grub
	echo 'menuentry "NickOS" { multiboot /boot/kernel.elf }' > $@

$(ISO_NAME): $(KERNEL_ELF) iso/boot/grub/grub.cfg
	cp $(KERNEL_ELF) iso/boot/kernel.elf
	grub-mkrescue -o $@ iso

run: $(ISO_NAME)
	qemu-system-x86_64 -cdrom $<

clean:
	rm -rf build iso $(ISO_NAME)
