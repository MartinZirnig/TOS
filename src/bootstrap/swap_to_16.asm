[bits 32]

%include "src/bootstrap/_constants.inc"

; >> void
; << void
swap_to_16:
    jmp         LONG_JUMP_16:       .protected16

[bits 16]
.protected16:
    mov         ax,                 SEGMENTATION_16
    mov         ds,                 ax
    mov         es,                 ax
    mov         fs,                 ax
    mov         gs,                 ax
    mov         ss,                 ax

    mov         eax,                cr0
    and         eax,                0x7FFFFFFF
    and         eax,                0xFFFFFFFE
    mov         cr0,                eax
    jmp         LONG_JUMP:          .pure_real

.pure_real:
    xor         ax,                 ax
    mov         ds,                 ax
    mov         es,                 ax
    mov         fs,                 ax
    mov         gs,                 ax
    mov         ss,                 ax
    cli