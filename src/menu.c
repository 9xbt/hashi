#include "text.h"
#include "kbd.h"
#include "util.h"
#include "menu.h"

extern void do_bios_call(int function, int extra);

#if 0
char *os_list[] = {
    "bentobox Version 0.1",
    "bentobox Version 0.1 [Serial debugging]",
    0
};
#endif

struct os os_list[10];

int longest_length(void) {
    int max = 0, len = 0;
    for (int i = 0; i < (signed)(sizeof(os_list) / sizeof(struct os)); i++) {
        len = strlen(os_list[i].name);
        if (len > max) {
            max = len;
        }
    }
    return max;
}

void clear_row(int count, int attr) {
    for (int j = 0; j < count; j++) {
        putchar(' ', attr);
    }
    putchar('\r', attr);
}

void show_menu(void) {
    puts(
        "hashi bootloader v0.1\n"
        "\n"
        "Please select the operating system to start:\n"
        "\n",
    0x07);
    for (; os_list[y - 4].name; y++);
    puts(
        "\n"
        "Use \030 and \031 to move the highlight to your choice.\n"
        "Press enter to choose and escape to exit.\n",
    0x07);

    int selection = 0;
    int longest = longest_length() + 8;
    for (;;) {
        x = 0; y = 4;

        for (int i = 0; os_list[i].name; i++) {
            clear_row(longest, i == selection ? 0x70 : 0x07);

            putchar('\t', i == selection ? 0x70 : 0x07);
            puts(os_list[i].name, i == selection ? 0x70 : 0x07);
            putchar('\n', i == selection ? 0x70 : 0x07);
        }

    readkey:
        int c = read_scancode();

        if (c == '\001') {
            do_bios_call(3, 0);
        } else if (c == 'H') {
            selection--;
        } else if (c == 'P') {
            selection++;
        } else goto readkey;

        if (selection < 0) selection = 0;
        if (!os_list[selection].name) selection--;
    }

    for (;;);
}