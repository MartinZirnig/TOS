ENTRY(__entry__)

KERNEL_VMA = 0xFFFFFFFF80000000;
KERNEL_LMA = 0x100000;

SECTIONS
{
    . = KERNEL_VMA;

    .text : AT(KERNEL_LMA)
    {
        __text_start = .;
        *(.entry)
        *(.text .text.*)
        *shared*__c_64.o(.text .text.*)
        *shared*__asm_64.o(.text .text.*)
        __text_end = .;
    }

    .rodata : AT(LOADADDR(.text) + SIZEOF(.text))
    {
        __rodata_start = .;
        *(.rodata .rodata.*)
        *shared*__c_64.o(.rodata .rodata.*)
        *shared*__asm_64.o(.rodata .rodata.*)
        __rodata_end = .;
    }

    .data : AT(LOADADDR(.rodata) + SIZEOF(.rodata))
    {
        __data_start = .;
        *(.data .data.*)
        *shared*__c_64.o(.data .data.*)
        *shared*__asm_64.o(.data .data.*)
        __data_end = .;
    }

    .bss (NOLOAD) :
    {
        __bss_start = .;
        *(.bss .bss.*)
        *(COMMON)
        *shared*__c_64.o(.bss .bss.*)
        *shared*__asm_64.o(.bss .bss.*)
        __bss_end = .;
    }

    /DISCARD/ :
    {
        *(.comment)
        *(.note.*)
        *(.eh_frame)
        *(.eh_frame_hdr)
        *(.gnu.*)
    }
}