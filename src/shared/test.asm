[bits 64]
global test
section .text
test:
    mov al, 'T'
    out 0xe9, al
    ret