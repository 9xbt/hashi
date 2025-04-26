int strcmp(const char *l, const char *r) {
	for (; *l == *r && *l; l++, r++);
	return *(unsigned char *)l - *(unsigned char *)r;
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