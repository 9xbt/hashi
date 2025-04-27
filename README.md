# hashi
hashi (箸) is a small multiboot-compatible x86 bootloader

## Features
- [X] El Torito boot support
- [X] Menu for selecting load option
- [X] Protected mode BIOS calls
    - [X] Set font
    - [X] Read disk
    - [X] Return control to BIOS
    - [X] Set video mode
- [ ] E820 memory map parsing
- [ ] VBE mode list parsing
- [X] ISO 9660 filesystem reading
- [ ] Multiboot kernel loading
    - [ ] Elf32 parser
- [X] Config parsing