#include <stdint.h>
#include "text.h"

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

extern void do_bios_call(int function);

void main() {
    memset(&_bss_start, 0, (uintptr_t)&_bss_end - (uintptr_t)&_bss_start);

    do_bios_call(5);
    puts("Hello, world!\n");

    for (;;) __asm__("hlt");
}