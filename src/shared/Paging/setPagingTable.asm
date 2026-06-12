global setPagingTable

section .text

%ifidn __OUTPUT_FORMAT__, elf64
[bits 64]

setPagingTable:
    mov cr3, rdi        ; In 64-bit, the 1st argument is in RDI
    ret

%elifidn __OUTPUT_FORMAT__, elf32
[bits 32]

setPagingTable:
    mov eax, [esp + 4]  ; In 32-bit, the 1st argument is on the stack
    mov cr3, eax        ; CR3 cannot be loaded directly from memory, must go through EAX
    ret

%endif