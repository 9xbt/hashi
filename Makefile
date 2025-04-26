# Toolchain
AS = nasm
CC = gcc

# Automatically find sources
S_SOURCES = $(shell cd src && find -L * -type f -name '*.S')
C_SOURCES = $(shell cd src && find -L * -type f -name '*.c')

# Get object files
OBJS := $(addprefix bin/, $(S_SOURCES:.S=.o) $(C_SOURCES:.c=.o))

# Flags
ASFLAGS = -f elf32
CCFLAGS = -m32 -std=c23 -Wall -Wextra -ffreestanding -nostdlib -fno-stack-protector -fno-stack-check -fno-lto -fno-pic -mno-sse -mno-sse2 -mno-red-zone
QEMUFLAGS = -debugcon stdio -cdrom bin/$(IMAGE_NAME).iso -boot d

# Output image name
IMAGE_NAME = hashi

all: boot iso

run: all
	@qemu-system-i386 $(QEMUFLAGS)

run-gdb: all
	@qemu-system-i386 $(QEMUFLAGS) -S -s

bin/%.o: src/%.S
	@echo " AS $<"
	@mkdir -p "$$(dirname $@)"
	@$(AS) $(ASFLAGS) -o $@ $<

bin/%.o: src/%.c
	@echo " CC $<"
	@mkdir -p "$$(dirname $@)"
	@$(CC) $(CCFLAGS) -c $< -o $@

boot: $(OBJS)
	@echo " LD bin/$(IMAGE_NAME).bin"
	@$(LD) -m elf_i386 -Tsrc/linker.ld $^ -o bin/$(IMAGE_NAME).bin

iso:
	@mkdir -p bin/iso
	@cp bin/$(IMAGE_NAME).bin bin/iso/boot.bin
	@mkisofs -quiet -b boot.bin -no-emul-boot -o bin/$(IMAGE_NAME).iso bin/iso/

clean:
	rm -f $(OBJS)
	rm -rf bin