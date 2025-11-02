#include <stdbool.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include <text.h>

void parse_num(char *s, int *ptr, int32_t val, uint32_t base, bool is_signed) {
    if (is_signed && val < 0) {
        s[(*ptr)++] = '-';
        val = -val;
    }

    uint32_t n = (uint32_t)val / base;
    uint32_t r = (uint32_t)val % base;

    if ((uint32_t)val >= base) {
        parse_num(s, ptr, n, base, false);
    }

    s[(*ptr)++] = (char)(r + '0');
}

void parse_hex(char *s, int *ptr, uint64_t val, int i) {
    while (i-- > 0) {
        s[(*ptr)++] = "0123456789abcdef"[(val >> (i * 4)) & 0x0F];
    }
}

void parse_string(char *s, int *ptr, const char *str) {
    if (!str) {
        memcpy(s + *ptr, "(null)", 6);
        *ptr += 6;
        return;
    }

    strcpy(s + *ptr, str);
    *ptr += strlen(str);
}

int vsprintf(char *s, const char *fmt, va_list args) {
    int ptr = 0;

    while (*fmt) {
        if (*fmt == '%') {
            fmt++;

            bool is_long = (*fmt == 'l') ? (fmt++, true) : false;

            switch (*fmt) {
                case 'u':
                    parse_num(s, &ptr, va_arg(args, int), 10, false);
                    break;
                case 'd':
                    parse_num(s, &ptr, va_arg(args, int), 10, true);
                    break;
                case 'x':
                    parse_hex(s, &ptr, is_long ? va_arg(args, uint64_t) : va_arg(args, uint32_t), is_long ? 16 : 8);
                    break;
                case 'p':
                    parse_hex(s, &ptr, va_arg(args, uint32_t), 8);
                    break;
                case 's':
                    parse_string(s, &ptr, va_arg(args, char *));
                    break;
                case 'c':
                    s[ptr++] = (char)va_arg(args, int);
                    break;
                case '%':
                    s[ptr++] = '%';
                    break;
            }
        } else {
            s[ptr++] = *fmt;
        }
        fmt++;
    }

    return ptr;
}

int printf(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    char buf[1024] = {0};
    int ret = vsprintf(buf, fmt, args);
    puts(buf, 0x07);
    va_end(args);

    return ret;
}