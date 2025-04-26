[BITS 16]

section .text.prologue
    extern main

    jmp 0x0:_start
_start:
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    
    mov [boot_disk], dl

    mov sp, 0x7c00

    mov ax, 3
    int 0x10
    
    cli

    in al, 0x92
    or al, 2
    out 0x92, al

    lgdt [gdtr]

    mov eax, cr0
    or al, 1
    mov cr0, eax

    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov fs, ax
    mov gs, ax

    jmp 0x08:main

boot_disk: db 0x00

gdtr:
    dw gdt_end - gdt_base - 1
    dd gdt_base
gdt_base:
    ; null segment
    dw 0x0000       ; Limit
    dw 0x0000       ; Base (low 16 bits)
    db 0x00         ; Base (mid 8 bits)
    db 00000000b    ; Access
    db 00000000b    ; Granularity
    db 0x00         ; Base (high 8 bits)

    ; 32-bit code
    dw 0xFFFF       ; Limit
    dw 0x0000       ; Base (low 16 bits)
    db 0x00         ; Base (mid 8 bits)
    db 10011011b    ; Access
    db 11001111b    ; Granularity
    db 0x00         ; Base (high 8 bits)

    ; 32-bit data
    dw 0xFFFF       ; Limit
    dw 0x0000       ; Base (low 16 bits)
    db 0x00         ; Base (mid 8 bits)
    db 10010010b    ; Access
    db 11001111b    ; Granularity
    db 0x00         ; Base (high 8 bits)

    ; 16-bit code
    dw 0xFFFF       ; Limit
    dw 0x0000       ; Base (low 16 bits)
    db 0x00         ; Base (mid 8 bits)
    db 10011110b    ; Access
    db 00000000b    ; Granularity
    db 0x00         ; Base (high 8 bits)
    
    ; 16-bit data
    dw 0xFFFF       ; Limit
    dw 0x0000       ; Base (low 16 bits)
    db 0x00         ; Base (mid 8 bits)
    db 10010010b    ; Access
    db 00000000b    ; Granularity
    db 0x00         ; Base (high 8 bits)
gdt_end:

times 510 - ($ - $$) db 0
dw 0xAA55

[BITS 32]

section .text
    global do_bios_call

do_bios_call:
    push ebp
    mov ebp, esp
    pushad

    mov ax, 0x20
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov fs, ax
    mov gs, ax

    jmp 0x18:do_bios_call.0
do_bios_call.0:
    mov eax, cr0
    and eax, ~1
    mov cr0, eax

    jmp 0x0:do_bios_call.1
[BITS 16]
do_bios_call.1:
    sti

    xor ax, ax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    mov ax, [bp + 8]
    cmp ax, 0x01
    je do_bios_call.set_font
    cmp ax, 0x02
    je do_bios_call.read_disk

    jmp do_bios_call.done
do_bios_call.read_disk:
    mov ah, 0x42
    mov dl, [boot_disk]
    mov si, dap
    int 0x13
    jmp do_bios_call.done
do_bios_call.set_font:
    mov ax, 0x1112
    mov bl, 0
    int 0x10
    jmp do_bios_call.done
do_bios_call.done:
    cli
    xor ax, ax
    mov ds, ax
    lgdt [gdtr]

    mov eax, cr0
    or al, 1
    mov cr0, eax

    jmp 0x08:do_bios_call.2
[BITS 32]
do_bios_call.2:
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov fs, ax
    mov gs, ax

    popad
    leave
    ret

section .data
    global dap_sectors
    global dap_buffer
    global dap_lba
    global disk_space

dap:
    db 0x10
    db 0x00
dap_sectors:
    dw 0x0000
dap_buffer:
    dd 0x00000000   ; segment:offset
dap_lba:
    dq 0x0000000000000000

disk_space:
    times 2048 db 0