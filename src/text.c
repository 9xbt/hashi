#include "util.h"
#define WIDTH  80
#define HEIGHT 25

int x, y;

void scroll(void) {
    static unsigned short *vmem = (unsigned short *)0xb8000;

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

void putchar(char c) {
    static unsigned short *vmem = (unsigned short *)0xb8000;

    switch (c) {
        case '\n':
            x = 0; y++;
            break;
        default:
            vmem[y * WIDTH + x++] = (0x07 << 8) | c;

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

void puts(char *str) {
    while (*str) {
        putchar(*str++);
    }
}