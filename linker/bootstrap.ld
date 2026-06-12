ENTRY(__entry__)

SECTIONS
{
    . = 0x8000;

    .text : ALIGN(4) {
        *bootstrap__asm.o(.text)
        
        *(.text)
        *(.text.*)
        
        *(.rodata)
        *(.rodata.*)
    }

    .data : ALIGN(4) {
        *(.data)
        *(.data.*)
    }

    .bss : ALIGN(4) {
        *(.bss)
        *(.bss.*)
        *(COMMON)
    }

    /DISCARD/ : {
        *(.comment)
        *(.note.*)
    }
}