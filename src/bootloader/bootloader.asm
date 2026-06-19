[BITS 16]

%include "src/bootloader/_constants.inc"

global __entry__

; >> void
; << void
__entry__:
    cli

    xor             ax, ax
    mov             ds, ax
    mov             es, ax
    mov             ss, ax
    mov             sp, STACK_TOP
    
    %include "src/bootloader/bootstrap_loader.asm"

    %include "src/bootloader/test_cpu.asm"

    %include "src/bootloader/mapping.asm"

    %include "src/bootloader/enable_protected_cpu_mode.asm"
    [bits 32]

    jmp BOOTSTRAP_JUMP_ADDRESS

; >> si -> message: char*
; << void
halt:
    cli
    hlt
fatal:
    call            print
    jmp             halt

%include "src/bootloader/print.asm"

; <========== BOOT SIGNATURE ==========>
times (TOTAL_SIZE - BOOT_MAGIC_SIZE) - ($ - $$) db NULL
dw BOOT_MAGIC