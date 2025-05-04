#pragma once
#define MAX_ENTRIES 10

struct os {
    char *name;
    char *path;
    char *cmdline;
};

void show_menu(void);