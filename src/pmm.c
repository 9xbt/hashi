#include <stdint.h>
#include <text.h>

struct e820_entry {
    uint64_t base;
    uint64_t len;
    uint32_t type;
    uint32_t acpi_extended;
} __attribute__((packed));

void pmm_install(void) {
    printf("pmm: printing memory map below:\n");
    struct e820_entry *entries = (struct e820_entry *)0x1004;
    for (int i = 0; i < *(uint16_t *)0x1000; i++) {
        struct e820_entry *entry = &entries[i];
        printf("pmm: base: %lx len: %lx type: %d\n", entry->base, entry->len, entry->type);
    }
}