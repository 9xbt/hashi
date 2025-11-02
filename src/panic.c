#include <stdarg.h>
#include <text.h>
#include <ps2.h>

void warning(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    char buf[1024] = {-1};
    vsprintf(buf, fmt, args);
    va_end(args);

    printf("hashi: %s\nPress any key to continue...", buf);
    read_scancode();
}

void panic(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    char buf[1024] = {-1};
    vsprintf(buf, fmt, args);
    va_end(args);

    printf("hashi: %s\n", buf);
    read_scancode();
    for (;;) asm("hlt");
}