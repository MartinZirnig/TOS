[bits 32]

%include "src/bootstrap/_constants.inc"

%define KERNEL_CODE_ACCESS (GDT_ACCESS_PRESENT | GDT_ACCESS_DESCRIPTOR_TYPE | GDT_ACCESS_EXECUTABLE | GDT_ACCESS_READ_WRITE)
%define KERNEL_DATA_ACCESS (GDT_ACCESS_PRESENT | GDT_ACCESS_DESCRIPTOR_TYPE | GDT_ACCESS_READ_WRITE)
%define FLAGS_32BIT_FULL   (GDT_FLAG_BYTE_OR_PAGE | GDT_FLAG_PROTECTED_MODE_ACTIVE | GDT_FLAG_LIMIT_16 | GDT_FLAG_LIMIT_17 | GDT_FLAG_LIMIT_18 | GDT_FLAG_LIMIT_19)

%define FLAGS_16BIT_PROTECTED (GDT_FLAG_LIMIT_16 | GDT_FLAG_LIMIT_17 | GDT_FLAG_LIMIT_18 | GDT_FLAG_LIMIT_19)

%define FLAGS_64BIT_CODE   (GDT_FLAG_LONG_MODE_ACTIVE)
%define FLAGS_64BIT_DATA   (0)


; >> void
; << void
updateGDT_start:
    lgdt            [GDT_DESCRIPTOR]

    mov             ax,             SEGMENTATION_32
    mov             ds,             ax
    mov             es,             ax
    mov             fs,             ax
    mov             gs,             ax
    mov             ss,             ax

    jmp             LONG_JUMP_32:   updateGDT_end

align 4
GDT:

    ; ==========================================================================
    ; 0x00: NULL DESCRIPTOR
    ; ==========================================================================
    GDT_UNION_NULL:                 dd 0, 0

    ; ==========================================================================
    ; 0x08: 32-BIT KERNEL CODE SEGMENT (Flat: Base=0, Limit=4GB)
    ; ==========================================================================
    GDT_U_CODE32:
        .LIMIT_LOW:                 dw 0xFFFF
        .BASE_LOW:                  dw 0x0000
        .BASE_MID:                  db 0x00
        .ACCESS:                    db KERNEL_CODE_ACCESS       ; 0x9A
        .FLAGS:                     db FLAGS_32BIT_FULL         ; 0xCF
        .BASE_HIGH:                 db 0x00

    ; ==========================================================================
    ; 0x10: 32-BIT KERNEL DATA SEGMENT (Flat: Base=0, Limit=4GB)
    ; ==========================================================================
    GDT_U_DATA32:
        .LIMIT_LOW:                 dw 0xFFFF
        .BASE_LOW:                  dw 0x0000
        .BASE_MID:                  db 0x00
        .ACCESS:                    db KERNEL_DATA_ACCESS       ; 0x92
        .FLAGS:                     db FLAGS_32BIT_FULL         ; 0xCF
        .BASE_HIGH:                 db 0x00

    ; ==========================================================================
    ; 0x18: 16-BIT KERNEL CODE SEGMENT (Chráněný pro de-elevaci: Limit=64KB)
    ; ==========================================================================
    GDT_U_CODE16:
        .LIMIT_LOW:                 dw 0xFFFF                   ; Limit je přesně 64 KB
        .BASE_LOW:                  dw 0x0000
        .BASE_MID:                  db 0x00
        .ACCESS:                    db KERNEL_CODE_ACCESS       ; 0x9A
        .FLAGS:                     db FLAGS_16BIT_PROTECTED    ; 0x0F (D=0 vyžaduje 16-bit zpracování!)
        .BASE_HIGH:                 db 0x00

    ; ==========================================================================
    ; 0x20: 16-BIT KERNEL DATA SEGMENT (Chráněný pro de-elevaci: Limit=64KB)
    ; ==========================================================================
    GDT_U_DATA16:
        .LIMIT_LOW:                 dw 0xFFFF
        .BASE_LOW:                  dw 0x0000
        .BASE_MID:                  db 0x00
        .ACCESS:                    db KERNEL_DATA_ACCESS
        .FLAGS:                     db FLAGS_16BIT_PROTECTED
        .BASE_HIGH:                 db 0x00

    ; ==========================================================================
    ; 0x28: 64-BIT KERNEL CODE SEGMENT (Long Mode)
    ; ==========================================================================
    GDT_U_CODE64:
        .LIMIT_LOW:                 dw 0x0000
        .BASE_LOW:                  dw 0x0000
        .BASE_MID:                  db 0x00
        .ACCESS:                    db KERNEL_CODE_ACCESS
        .FLAGS:                     db FLAGS_64BIT_CODE
        .BASE_HIGH:                 db 0x00

    ; ==========================================================================
    ; 0x30: 64-BIT KERNEL DATA SEGMENT (Long Mode)
    ; ==========================================================================
    GDT_U_DATA64:
        .LIMIT_LOW:                 dw 0x0000
        .BASE_LOW:                  dw 0x0000
        .BASE_MID:                  db 0x00
        .ACCESS:                    db KERNEL_DATA_ACCESS
        .FLAGS:                     db FLAGS_64BIT_DATA
        .BASE_HIGH:                 db 0x00
GDT_UNIVERSAL_END:

GDT_DESCRIPTOR:
    dw GDT_UNIVERSAL_END - GDT - 1
    dd GDT

updateGDT_end: