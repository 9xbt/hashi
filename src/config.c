#include <stdint.h>
#include "text.h"
#include "menu.h"
#include "util.h"
#include "iso9660.h"

extern int read_disk(char *buffer, uint64_t lba);
extern void do_bios_call(int function, int extra);

int kmain(void) {
    char data_base[ISO_SECTOR_SIZE];
    for (int i = 0x10; i < 0x15; i++) {
        read_disk(data_base, i);
        root = (void *)data_base;
        if (root->type == 1) {
            goto done;
        }
    }
    puts("hashi: No ISO 9660 filesystem found!\n", 0x07);
    for (;;) __asm__("hlt");

done:
    if (!navigate("BOOT.INI")) {
        puts("hashi: BOOT.INI not found!\n", 0x07);
        for (;;) __asm__("hlt");
    }

    char *config = (char *)(DATA_LOAD_BASE + dir_entry->extent_start_LSB * ISO_SECTOR_SIZE);
    read(config);

    char *line = config;
    int entry = -1;
    extern struct os os_list[10];
    while (*config) {
        if (*config == '\n' || *config == '\r') {
            if (line[0] != ';' && line[0] != '#') {
                *config = 0;
                if (line[0] == '[') {
                    char *pos = strchr(line, ']');
                    if (pos) {
                        *pos = 0;
                        entry++;
                        os_list[entry].name = line + 1;
                    }
                } else if (!strncmp(line, "mode = ", 7)) {
                    char *mode = line + 7;
                    if (!strcmp(mode, "80x50")) {
                        do_bios_call(1, 0);
                    }
                }
            }
            line = config + 1;
        }
        config++;
    }

    for (;;) {
	    show_menu();
    }
    return 0;
}