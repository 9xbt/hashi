OUTPUT_FORMAT("binary")

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

    _bss_start = .;
    .bss : {
        *(.bss)
    }
    _bss_end = .;

    /DISCARD/ :
	{
		*(.comment)
		*(.eh_frame)
		*(.note.gnu.build-id)
	}
}