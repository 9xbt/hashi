#include "util.h"
#include "text.h"
#define WIDTH  80
#define HEIGHT 25

int x, y;
static unsigned short *vmem = (unsigned short *)0xb8000;

extern void do_bios_call(int function, int extra);

void scroll(void) {
    for (int i = 0; i < WIDTH * (HEIGHT - 1); i++)
        vmem[i] = vmem[i + WIDTH];
    for (int i = 0; i < WIDTH; i++)
        vmem[WIDTH * (HEIGHT - 1) + i] = 0x07 << 8;
    y--;
}

void update_cursor(void) {
    unsigned short pos = y * WIDTH + x;
    outb(0x3d4, 0x0f);
    outb(0x3d5, pos & 0xff);
    outb(0x3d4, 0x0e);
    outb(0x3d5, pos >> 8);
}

void clear(int attr) {
    for (int i = 0; i < 80 * 25; i++)
        vmem[i] = attr << 8;
    x = 0, y = 0;
    update_cursor();
}

void putchar(char c, int attr) {
    switch (c) {
        case '\n':
            x = 0; y++;
            break;
        case '\r':
            x = 0;
            break;
        case '\b':
            if (x > 0) x--;
            break;
        case '\t':
            puts("    ", attr);
            break;
        default:
            vmem[y * WIDTH + x++] = (attr << 8) | c;

            if (x >= WIDTH) {
                x = 0; y++;
            }
            break;
    }

    if (y >= HEIGHT) {
        scroll();
    }

    update_cursor();
}

void puts(char *str, int attr) {
    while (*str) {
        putchar(*str++, attr);
    }
}