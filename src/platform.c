#include <stdint.h>
#include "text.h"
#include "util.h"
#include "config.h"
#include "iso9660.h"

extern char _bss_start[];
extern char _bss_end[];

void *memcpy(void *restrict dest, const void *restrict src, long n) {
	__asm__ volatile("cld; rep movsb"
	            : "=c"((int){0})
	            : "D"(dest), "S"(src), "c"(n)
	            : "flags", "memory");
	return dest;
}

void *memset(void *dest, int c, long n) {
	__asm__ volatile("cld; rep stosb"
	             : "=c"((int){0})
	             : "D"(dest), "a"(c), "c"(n)
	             : "flags", "memory");
	return dest;
}

extern void do_bios_call(int function, int extra);

void text_reset(void) {
	outb(0x3D4, 0x0A);
    outb(0x3D5, 0x20);
}

extern volatile uint16_t dap_sectors;
extern volatile uint32_t dap_buffer;
extern volatile uint32_t dap_lba_low;
extern volatile uint32_t dap_lba_high;
extern volatile uint16_t drive_params_bps;
extern uint8_t disk_space[];

int read_disk(char *buffer, uint64_t lba) {
	dap_sectors = 2048 / drive_params_bps;
	dap_buffer = (uint32_t)disk_space;
	dap_lba_low = lba * dap_sectors;
	dap_lba_high = 0;
	do_bios_call(2, 0);
	memcpy(buffer, disk_space, 2048);
	return 0;
}

iso_9660_volume_descriptor_t *root = nullptr;
iso_9660_directory_entry_t *dir_entry = nullptr;
static char *dir_entries = nullptr;

int main() {
    memset(&_bss_start, 0, (uintptr_t)&_bss_end - (uintptr_t)&_bss_start);
	text_reset();

	return kmain();
}

void spin(void) {
	static int spin_i = 0;
	putchar("/-\\|"[spin_i], 0x07);
	putchar('\b', 0x07);
}

int navigate(char *name) {
	dir_entry = (iso_9660_directory_entry_t *)&root->root;

	dir_entries = (char *)(DATA_LOAD_BASE + dir_entry->extent_start_LSB * ISO_SECTOR_SIZE);
	read_disk(dir_entries, dir_entry->extent_start_LSB);
	unsigned long offset = 0;
	for (;;) {
		iso_9660_directory_entry_t *dir = (iso_9660_directory_entry_t *)(dir_entries + offset);
		if (dir->length == 0) {
			if (offset < dir_entry->extent_length_LSB) {
				offset += 1;
				goto try_again;
			}
			break;
		}
		if (!(dir->flags & FLAG_HIDDEN)) {
			char file_name[dir->name_len + 1];
			memcpy(file_name, dir->name, dir->name_len);
			file_name[dir->name_len] = 0;
			char *s = strchr(file_name,';');
			if (s) {
				*s = '\0';
			}
			if (!strcmp(file_name, name)) {
				dir_entry = dir;
				return 1;
			}
		}
		offset += dir->length;
try_again:
		if (offset > dir_entry->extent_length_LSB) break;
	}

	return 0;
}

void read(char *start) {
	for (unsigned int i = 0, j = 0; i < dir_entry->extent_length_LSB; i += ISO_SECTOR_SIZE, j++) {
		if (!(j & 0x3FF)) spin();
		read_disk(start + i, dir_entry->extent_start_LSB + j);
	}
}