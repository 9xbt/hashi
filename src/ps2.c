#include <stdbool.h>
#include <string.h>
#include <ps2.h>

const char kb_map_keys[128] = {
    0, 27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0,
    '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, '*', 0, ' ',
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '-', 0, 0, 0, '+', 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0
};

const char kb_map_shift[128] = {
    0,  27, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b',
    '\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n',
    0, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~', 0,
    '\\', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', 0, '*', 0, ' ',
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '-', 0, 0, 0, '+', 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0
};

const char kb_map_caps[128] = {
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '[', ']', '\n',
    0, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ';', '\'', '`', 0,
    '\\', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', ',', '.', '/', 0, '*', 0, ' ',
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '-', 0, 0, 0, '+', 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0
};

void wait_keyboard(void) {
	while (!(inb(0x64) & 1)) {
		outb(0x80, 0);
	}
}

int read_scancode(void) {
	static bool keys[256];
	wait_keyboard();

	unsigned char sc = inb(0x60);
	if (sc == 0xe0) {
		wait_keyboard();
		sc = inb(0x60);
	}
	
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

	if (sc & 0x80)
		return 0;
	if (shift)
		return kb_map_shift[sc & 0x7f];
	if (caps)
		return kb_map_caps[sc & 0x7f];
	return kb_map_keys[sc & 0x7f];
}