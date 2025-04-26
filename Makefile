# Assembler
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
QEMUFLAGS = -debugcon stdio -drive file=bin/$(IMAGE_NAME),format=raw

# Output image name
IMAGE_NAME = hashi.img

all: boot

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
	@echo " LD bin/$(IMAGE_NAME)"
	@$(LD) -m elf_i386 -Tsrc/linker.ld $^ -o bin/$(IMAGE_NAME)

clean:
	rm -f $(OBJS)
	rm -rf bin