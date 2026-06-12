[BITS 16]

%include "src/bootloader/_constants.inc"

extern fatal


; >> si -> message: char*
; << void
print:
    push            ax
    push            si
.loop:
    lodsb
    cmp             al, 0
    je              .done
    mov             ah, PRINT_BIOS_CALL
    int 0x10
    jmp             .loop
.done:
    pop             si
    pop             ax
    ret