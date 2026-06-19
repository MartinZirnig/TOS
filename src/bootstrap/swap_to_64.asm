[bits 32]

%include "src/bootstrap/_constants.inc"

PAGING_BIT              equ (1 << 31)
WRITE_PROTECT_BIT       equ (1 << 16)
EFER_ADDRESS            equ 0xC0000080
EFER_NXE_FLAG           equ 1 << 11
LONG_MODE_ENABLE_FLAG   equ 1 << 8
PAE_FLAG                equ 1 << 5

; >> PML4* in eax
; << void
swap_to_64:
    mov             ecx,            cr4
    or              ecx,            PAE_FLAG
    mov             cr4,            ecx

    mov             cr3,            eax

    mov             ecx,            EFER_ADDRESS
    rdmsr
    or              eax,            LONG_MODE_ENABLE_FLAG | EFER_NXE_FLAG
    wrmsr

    mov             ecx,            cr0
    or              ecx,            PAGING_BIT | WRITE_PROTECT_BIT
    mov             cr0,            ecx

    jmp             LONG_JUMP_64:   .long_mode_start

[bits 64]
.long_mode_start:
    mov             ax,             SEGMENTATION_64
    mov             ds,             ax
    mov             es,             ax
    mov             fs,             ax
    mov             gs,             ax
    mov             ss,             ax

    cli