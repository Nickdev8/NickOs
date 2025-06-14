ISO_NAME=NickOS.iso
KERNEL_ELF=build/kernel.elf

all: iso/boot/grub/grub.cfg $(ISO_NAME)

build:
	mkdir -p build

build/boot.o: src/boot.asm | build
	nasm -f elf32 $< -o $@

build/kernel.o: src/kernel.c | build
	x86_64-elf-gcc -m32 -ffreestanding -c $< -o $@

$(KERNEL_ELF): build/boot.o build/kernel.o
	x86_64-elf-ld -m elf_i386 -T link.ld $^ -o $@

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
