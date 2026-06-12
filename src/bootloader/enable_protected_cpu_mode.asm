[BITS 16]

%include "src/bootloader/_constants.inc"

extern fatal
global enable_protected_cpu_mode_start
global enable_protected_cpu_mode_finish

%define KERNEL_CODE_ACCESS (GDT_ACCESS_PRESENT | GDT_ACCESS_DESCRIPTOR_TYPE | GDT_ACCESS_EXECUTABLE | GDT_ACCESS_READ_WRITE)
%define KERNEL_DATA_ACCESS (GDT_ACCESS_PRESENT | GDT_ACCESS_DESCRIPTOR_TYPE | GDT_ACCESS_READ_WRITE)
%define FLAGS_32BIT_FULL   (GDT_FLAG_BYTE_OR_PAGE | GDT_FLAG_PROTECTED_MODE_ACTIVE | GDT_FLAG_LIMIT_16 | GDT_FLAG_LIMIT_17 | GDT_FLAG_LIMIT_18 | GDT_FLAG_LIMIT_19)

; >> void
; << void
enable_protected_cpu_mode_start:
    cli
    
    lgdt            [GDT_DESCRIPTOR]

    ; cr0 zero bit indicates protected mode
    mov             eax, cr0
    or              eax, 1
    mov             cr0, eax

    jmp             FAR_JUMP_PREFFIX:init_protected_mode


[BITS 32]
init_protected_mode:
    ; redirect 16bit segmentation to gdt
    mov             ax, SEGMENTATION_ADDRESS
    mov             ds, ax
    mov             es, ax
    mov             fs, ax
    mov             gs, ax
    mov             ss, ax    

    jmp             enable_protected_cpu_mode_finish             

align 4
GDT_START:
    ; required constants
    GDT_NULL_DESCRIPTOR:            dd 0, 0

    GDT_KERNEL_CODE_SEGMENT:    ; (Index 1 -> Offset 0x08)
        GDT_KCS_LIMIT:              dw GDT_LIMIT
        GDT_KCS_BASE_LOW:           dw GDT_BASE
        GDT_KCS_BASE_MID:           db GDT_BASE
        GDT_KCS_ACCESS:             db KERNEL_CODE_ACCESS
        GDT_KCS_FLAGS:              db FLAGS_32BIT_FULL
        GDT_KCS_BASE_HIG:           db GDT_BASE

    GDT_KERNEL_DATA_SEGMENT:    ;(Index 2 -> Offset 0x10)
        GDT_KDS_LIMIT:              dw GDT_LIMIT
        GDT_KDS_BASE_LOW:           dw GDT_BASE
        GDT_KDS_BASE_MID:           db GDT_BASE
        GDT_KDS_ACCESS:             db KERNEL_DATA_ACCESS
        GDT_KDS_FLAGS:              db FLAGS_32BIT_FULL
        GDT_KDS_BASE_HIG:           db GDT_BASE
GDT_END:

GDT_DESCRIPTOR:
    GDTD_DESCRIPTOR_SIZE:           dw GDT_END - GDT_START - 1
    GDTD_DESCRIPOT_ADDR:            dd GDT_START

enable_protected_cpu_mode_finish: