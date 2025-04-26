#include <stdint.h>
#include "menu.h"
#include "util.h"

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

void text_reset(void) {
	outb(0x3D4, 0x0A);
    outb(0x3D5, 0x20);
}

void main() {
    memset(&_bss_start, 0, (uintptr_t)&_bss_end - (uintptr_t)&_bss_start);
	text_reset();

	show_menu();

    for (;;) __asm__("hlt");
}