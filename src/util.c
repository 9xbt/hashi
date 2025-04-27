int strcmp(const char *l, const char *r) {
	for (; *l == *r && *l; l++, r++);
	return *(unsigned char *)l - *(unsigned char *)r;
}

int strncmp(const char *x, const char *y, unsigned long n) {
    unsigned char u1, u2;

    while (n-- > 0) {
        u1 = (unsigned char) *x++;
        u2 = (unsigned char) *y++;
        if (u1 != u2)
	        return u1 - u2;
        if (u1 == '\0')
	        return 0;
    }
    return 0;
}

char *strchr(const char *s, int c) {
	while (*s) {
		if (*s == c) {
			return (char *)s;
		}
		s++;
	}
	return 0;
}

unsigned long strlen(const char *s) {
	unsigned long out = 0;
	while (*s) {
		out++;
		s++;
	}
	return out;
}