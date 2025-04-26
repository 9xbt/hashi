#include "util.h"
#include "kbd.h"

int read_scancode(void) {
	static bool keys[256];
	while (!(inb(0x64) & 1)) {
		outb(0x80, 0);
	}

	unsigned char sc = inb(0x60);
	
	if (sc & 0x80) {
		sc &= 0x7f;
		keys[sc] = false;
		return sc | 0x80;
	}
	if (keys[sc]) {
		return -1;
	}
	keys[sc] = true;
	return sc;
}

int read_key(void) {
	static bool shift = false, caps = false;

	int sc = read_scancode();
	switch (sc) {
		case 0x2a:
		case 0x36:
			shift = true;
			return 0;
		case 0xaa:
		case 0xb6:
			shift = false;
			return 0;
		case 0x3a:
			caps = !caps;
			return 0;
	}

	if (sc & 0x80) {
		return 0;
	}

	if (shift) {
		return kb_map_shift[sc & 0x7f];
	} else if (caps) {
		return kb_map_caps[sc & 0x7f];
	}
	return kb_map_keys[sc & 0x7f];
}