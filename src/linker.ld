OUTPUT_FORMAT("binary")
ENTRY(_start)

SECTIONS
{
    . = 0x7c00;

    .text : {
        *(.text.prologue)
        *(.text)
    }

    .rodata : {
        *(.rodata)
    }

    .data : {
        *(.data)
    }

    .bss : {
        *(.bss)
    }

    /DISCARD/ :
	{
		*(.comment)
		*(.eh_frame)
		*(.note.gnu.build-id)
	}
}