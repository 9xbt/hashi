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

int main() {
    memset(&_bss_start, 0, (uintptr_t)&_bss_end - (uintptr_t)&_bss_start);
	text_reset();

	return kmain();
}