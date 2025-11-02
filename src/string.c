#include <stdint.h>
#include <stddef.h>

void *memcpy(void *restrict dest, const void *restrict src, size_t n) {
	asm volatile("cld; rep movsb"
	            : : "D"(dest), "S"(src), "c"(n)
	            : "flags", "memory");
	return dest;
}

void *memset(void *dest, int c, size_t n) {
    asm volatile("cld; rep stosb"
                : : "D"(dest), "a"(c), "c"(n)
                : "flags", "memory");
    return dest;
}

size_t strlen(const char *s) {
    size_t i = 0;
    while (*s != '\0') {
        i++;
        s++;
    }
    return i;
}

int strcmp(const char *a, const char *b) {
    while (*a && (*a == *b)) {
        a++;
        b++;
    }
    return *(unsigned char *)a - *(unsigned char *)b;
}

int strncmp(const char *a, const char *b, register size_t n) {
    unsigned char u1, u2;

    while (n-- > 0) {
        u1 = (unsigned char) *a++;
        u2 = (unsigned char) *b++;
        if (u1 != u2)
	        return u1 - u2;
        if (u1 == '\0')
	        return 0;
    }
    return 0;
}

char *strchr(register const char *s, int c) {
    do {
        if (*s == c) {
            return (char *)s;
        }
    } while (*s++);
    return 0;
}

char *strcpy(char *dest, const char *src) {
    if (!dest)
        return NULL;
 
    char *ptr = dest;
    while (*src != '\0') {
        *dest = *src;
        dest++;
        src++;
    }
 
    *dest = '\0';
    return ptr;
}