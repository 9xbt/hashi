#pragma once
#include <stdarg.h>

extern int x, y;

void clear(int attr);
void putchar(char c, int attr);
void puts(char *str, int attr);
void disable_cursor(void);
void enable_cursor(void);

int vsprintf(char *s, const char *fmt, va_list args);
int printf(const char *fmt, ...);

void warning(const char *fmt, ...);
void panic(const char *fmt, ...);