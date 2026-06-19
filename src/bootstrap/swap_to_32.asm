[bits 16]

%include "src/bootstrap/_constants.inc"

; >> void
; << void 
swap_to_32:
    mov             eax,        cr0
    or              eax,        1
    mov             cr0,        eax

    jmp LONG_JUMP_32:.re_elevated_32

[bits 32]
.re_elevated_32:
    mov             ax,         SEGMENTATION_32
    mov             ds,         ax
    mov             es,         ax
    mov             fs,         ax
    mov             gs,         ax
    mov             ss,         ax
    cli