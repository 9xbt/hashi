#include "multiboot.h"
#include "kbd.h"
#include "menu.h"
#include "text.h"
#include "iso9660.h"
#include "util.h"
#include "elf.h"

void *kernel = nullptr;
void *xmain = nullptr;

void panic(char *s, char *extra) {
    puts("hashi: ", 0x07);
    puts(s, 0x07);
    if (extra) puts(extra, 0x07);
    puts("Press any key to continue...", 0x07);

    while (read_scancode() & 0x80) {
        __asm__("pause");
    }
}

bool check_multiboot(void) {
    for (uintptr_t off = 0; off < 0x2000; off += sizeof(uintptr_t)) {
        if (*((uint32_t *)(kernel + off)) == MULTIBOOT_MAGIC) {
            return true;
        }
    }
    return false;
}

void print_hex(uint32_t val) {
    int i = 8;
    while (i-- > 0) {
        putchar("0123456789abcdef"[val >> (i * 4) & 0x0F], 0x07);
    }
}

bool load_elf(void) {
    Elf32_Ehdr *ehdr = (Elf32_Ehdr *)kernel;
    if (strncmp((char *)ehdr->e_ident, "\x7f""ELF", 4) ||
        ehdr->e_ident[EI_CLASS] != ELFCLASS32) {
        return false;
    }

    xmain = (void *)ehdr->e_entry;
    Elf32_Phdr *phdr = (Elf32_Phdr *)((uint8_t *)kernel + ehdr->e_phoff);

    for (int i = 0; i < ehdr->e_phnum; i++) {
        if (phdr[i].p_type == PT_LOAD) {
            if (phdr[i].p_filesz > 0) {
                uintptr_t src = (uintptr_t)kernel + phdr[i].p_offset;
                uintptr_t dest = phdr[i].p_paddr;

                print_hex(dest);
                memcpy((void *)dest, (void *)src, phdr[i].p_filesz);
            }
        }
    }

    return true;
}

void boot([[maybe_unused]] struct os *os) {
    clear(0x07);
    if (!navigate(os->path)) {
        panic(os->path, " not found!\n");
        return;
    }

    kernel = (char *)(DATA_LOAD_BASE + dir_entry->extent_start_LSB * ISO_SECTOR_SIZE);
    read(kernel);

    if (!check_multiboot()) {
        panic("Multiboot header not found!\n", nullptr);
        return;
    }

    if (!load_elf()) {
        panic("Failed to load ELF!\n", nullptr);
        return;
    }

    __asm__ __volatile__ (
		"jmp *%0" :: "r"(xmain), "a"(MULTIBOOT_EAX_MAGIC), "b"(nullptr));

    for (;;) __asm__("hlt");
}