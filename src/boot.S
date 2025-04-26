[BITS 16]

section .text.prologue
    global _start
    extern kmain

_start:
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax

    mov sp, 0x7c00

    mov ah, 0x42
    mov si, dap
    int 0x13
    
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

    jmp 0x08:kmain

dap:
    db 0x10         ; size of packet
    db 0x00         ; unused
    dw 0x0008       ; sector number
    dd 0x00007E00   ; segment:offset
    dq 0x01         ; lba address

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
    db 10010011b    ; Access
    db 11001111b    ; Granularity
    db 0x00         ; Base (high 8 bits)
gdt_end:

times 510 - ($ - $$) db 0
dw 0xAA55

;mov al, 'H'
;mov ah, 0xE
;int 0x10

;mov al, 'i'
;int 0x10

;hlt