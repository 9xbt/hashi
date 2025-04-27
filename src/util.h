#pragma once

#define DATA_LOAD_BASE 0x4000000

static inline void outb(unsigned short port, unsigned char val) {
    __asm__ volatile ("outb %0, %1" : : "a"(val), "Nd"(port) : "memory");
}

static inline unsigned char inb(unsigned short port) {
    unsigned char ret;
    __asm__ volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port) : "memory");
    return ret;
}

void *memcpy(void *restrict dest, const void *restrict src, long n);
void *memset(void *dest, int c, long n);
int strcmp(const char *l, const char *r);
int strncmp(const char *l, const char *r, unsigned long n);
char *strchr(const char *s, int c);
unsigned long strlen(const char *s);