#include "util.h"

void puts(char *str) {
    while (*str) {
        outb(0xe9, *str++);
    }
}