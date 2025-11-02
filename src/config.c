#include <stdint.h>
#include <text.h>
#include <menu.h>
#include <string.h>
#include <iso9660.h>
#include <ps2.h>

#define INI_PATH "BOOT.INI"

extern int read_disk(char *buffer, uint64_t lba);
extern void do_bios_call(int function, int extra);

void parse_mode(char *mode, [[maybe_unused]] struct os *os) {
    if (!strcmp(mode, "80x50")) {
        do_bios_call(1, 0);
    } else if (strcmp(mode, "80x25")) {
        warning("Invalid mode: ", mode);
    }
}

void parse_path(char *path, struct os *os) {
    os->path = path;
}

void parse_cmdline(char *cmdline, struct os *os) {
    os->cmdline = cmdline;
}

void parse_config(void) {
    char data_base[ISO_SECTOR_SIZE];
    for (int i = 0x10; i < 0x15; i++) {
        read_disk(data_base, i);
        root = (void *)data_base;
        if (root->type == 1) {
            goto done;
        }
    }
    panic("No ISO 9660 filesystem found!");

done:
    if (!navigate(INI_PATH)) {
        panic(INI_PATH" not found!");
    }

    char *config = (char *)(DATA_LOAD_BASE + dir_entry->extent_start_LSB * ISO_SECTOR_SIZE);
    read(config);

    char *line = config;
    int entry = -1;
    extern struct os os_list[10];
    while (*config) {
        if (*config == '\n' || *config == '\r') {
            if (line[0] != ';' && line[0] != '#') {
                #define MATCH(r,h) if (!strncmp(line, r " = ", strlen(r)+3)) { h(line + strlen(r)+3, &os_list[entry]); }

                *config = 0;
                if (line[0] == '[') {
                    char *pos = strchr(line, ']');
                    if (pos) {
                        *pos = 0;
                        entry++;
                        os_list[entry].name = line + 1;
                    }
                }
                MATCH("mode", parse_mode);
                MATCH("kernel", parse_path);
                MATCH("cmdline", parse_cmdline);
            }
            line = config + 1;
        }
        config++;
    }
}