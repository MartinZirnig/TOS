ENTRY(__entry__)
SECTIONS {
    . = 0xFFFFFFFF80000000;

    .text : {
        *(.entry)
        *(.multiboot)
        *(.text*)
    }
    .rodata : { *(.rodata*) }
    .data   : { *(.data*) }
    .bss    : { *(.bss*) }
}