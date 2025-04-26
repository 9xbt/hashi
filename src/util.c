unsigned long strlen(const char *s) {
	unsigned long out = 0;
	while (*s) {
		out++;
		s++;
	}
	return out;
}