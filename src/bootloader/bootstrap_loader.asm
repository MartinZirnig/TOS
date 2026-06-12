[BITS 16]

%include "src/bootloader/_constants.inc"

extern fatal

; >> dl -> boot drive id : byte
; << [0x0000:0x8000] -> loaded bootstrap 
bootstrap_loader_start:
    xor         ax, ax
    mov         ds, ax          ; ZAISTÍME, ŽE DS JE STOPROCENTNĚ 0!
    mov         es, ax          ; Pro jistotu i ES

    mov         si, DISK_ADDRESS_PACKET
    mov         ah, READ_DISK
    int 0x13

    mov         si, BOOTSTRAP_LOAD_ERROR
    jc          fatal
    jmp         bootstrap_loader_finish


BOOTSTRAP_LOAD_ERROR:           db "Cannot load bootstrap phase!", 13, 10, 0

align 4
DISK_ADDRESS_PACKET:
    DAP_SIZE:                   db 0x10
    DAP_RESERVED:               db NULL
    DAP_SECTORS_COUNT:          dw BOOTSTRAP_SIZE
    DAP_RAM_ADDRESS:            dw BOOTSTRAP_ADDRESS
    DAP_RAM_SECTOR:             dw BOOTSTRAP_SECTOR
    DAM_FIRST_DISK_SECTOR:      dq 1

bootstrap_loader_finish:
