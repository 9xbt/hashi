#include <ps2.h>
#include <stdint.h>
#include <stddef.h>
#include <text.h>
#include <string.h>
#include <config.h>
#include <iso9660.h>
#include <menu.h>
#include <pmm.h>

extern char _bss_start[];
extern char _bss_end[];

extern void do_bios_call(int function, int extra);

extern volatile uint16_t dap_sectors;
extern volatile uint32_t dap_buffer;
extern volatile uint32_t dap_lba_low;
extern volatile uint32_t dap_lba_high;
extern volatile uint16_t drive_params_bps;
extern uint8_t disk_space[];

int read_disk(char *buffer, uint64_t lba) {
	dap_sectors = 2048 / drive_params_bps;
	dap_buffer = (uint32_t)disk_space;
	dap_lba_low = lba * dap_sectors;
	dap_lba_high = 0;
	do_bios_call(2, 0);
	memcpy(buffer, disk_space, 2048);
	return 0;
}

iso_9660_volume_descriptor_t *root = NULL;
iso_9660_directory_entry_t *dir_entry = NULL;
static char *dir_entries = NULL;

int main() {
    memset(&_bss_start, 0, (uintptr_t)&_bss_end - (uintptr_t)&_bss_start);
	disable_cursor();

    pmm_install();
    parse_config();
    read_scancode();

    show_menu();
    panic("hit unreachable code");
    __builtin_unreachable();
}

void spin(void) {
	static int i = -1;
	putchar("/-\\|"[i++], 0x07);
	putchar('\b', 0x07);
}

int navigate(char *name) {
    dir_entry = (iso_9660_directory_entry_t *)&root->root;
    
    dir_entries = (char *)(DATA_LOAD_BASE + dir_entry->extent_start_LSB * ISO_SECTOR_SIZE);
    read_disk(dir_entries, dir_entry->extent_start_LSB);
    
    uint32_t offset = 0;
    uint32_t extent_length = dir_entry->extent_length_LSB;
    while (offset < extent_length) {
        iso_9660_directory_entry_t *dir = (iso_9660_directory_entry_t *)(dir_entries + offset);
        if (dir->length == 0) {
            offset++;
            continue;
        }
        
        if (!(dir->flags & FLAG_HIDDEN)) {
            char filename[dir->name_len + 1];
            memcpy(filename, dir->name, dir->name_len);
            filename[dir->name_len] = '\0';
            char *s = strchr(filename, ';');
            if (s) {
                *s = '\0';
            }
            if (strcmp(filename, name) == 0) {
                dir_entry = dir;
                return 1;
            }
        }
        offset += dir->length;
    }
    return 0;
}

void read(char *start) {
	for (uint32_t i = 0, j = 0; i < dir_entry->extent_length_LSB; i += ISO_SECTOR_SIZE, j++) {
		spin();
		read_disk(start + i, dir_entry->extent_start_LSB + j);
	}
}