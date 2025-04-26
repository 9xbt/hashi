#include "util.h"
#include "text.h"
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

void putchar(char c, int attr) {
    static unsigned short *vmem = (unsigned short *)0xb8000;

    switch (c) {
        case '\n':
            x = 0; y++;
            break;
        case '\r':
            x = 0;
            break;
        case '\b':
            if (x > 0) x--;
            vmem[y * WIDTH + x] = attr << 8;
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