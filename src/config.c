#include <stdint.h>
#include "text.h"
#include "menu.h"
#include "iso9660.h"

extern int read_disk(char *buffer, uint64_t lba);

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
    for (;;) {
	    show_menu();
    }
    return 0;
}