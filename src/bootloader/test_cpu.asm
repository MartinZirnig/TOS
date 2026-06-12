[BITS 16]

%include "src/bootloader/_constants.inc"

extern fatal
 
; >> void
; << void
test_cpu_start:
    ; test if CPU supports extended calls
    mov             eax, EXTENDED_SUPPORT_CALL
    cpuid
    cmp             eax, SUPPORTS_64BIT_CALL
    jb              .error

    ; test if CPU supports 64bit mode
    mov             eax, SUPPORTS_64BIT_CALL
    cpuid
    test            edx, SUPPORTS_64BIT_FLAG     
    jz              .error

    jmp             test_cpu_finish

.error:
    mov             si, TEST_CPU_ERROR
    jmp             fatal

TEST_CPU_ERROR:             db 'CPU does not support 64bit mode!', 13, 10, 0
test_cpu_finish: