ENTRY(__entry__)
SECTIONS {
    . = 0x7C00;

    .text : {
        *(.entry)
        *(.text*)
    }
    .rodata : { *(.rodata*) }
    .data   : { *(.data*) }
    .bss    : { *(.bss*) }

    /* Volitelné: Bezpečnostní kontrola, aby bootloader nepřesáhl 512 bajtů */
    /DISCARD/ : { *(*) }
}