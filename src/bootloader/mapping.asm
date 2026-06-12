[BITS 16]

%include "src/bootloader/_constants.inc"

extern fatal


; >> void
; << [0x0000:0x0500] -> mapped memory metadata
; << [0x0000:0x0502] -> mapped memory map
mapping_start:
    xor             ax, ax
    mov             es, ax
    xor             ebx, ebx
    xor             bp, bp 
    mov             di, MAPPING_DATA_START

.loop:
    mov             eax, MMAP_BIOS_CALL
    mov             edx, SMAP_CALL_MAGIC
    mov             ecx, MMAP_STRUCT_SIZE
    int 0x15

    jc              .done               

    ; test if function is supported
    cmp             eax, SMAP_CALL_MAGIC
    jne             .error

    ; Empty memory segment, skip and repeat
    jcxz            .skip_entry

    inc             bp
    add             di, MMAP_STRUCT_SIZE

.skip_entry:
    test            ebx, ebx
    jz              .done
    jmp             .loop

.error:
    mov             si, MAPPING_ERROR
    jmp             fatal

.done:
    mov             [MAPPING_META_START], bp
    jmp             mapping_finish

MAPPING_ERROR:          db 'Memory mapping not supported', 13, 10, 0

mapping_finish: