section .text
global __entry__
extern __kill__
extern test
extern test_c

__entry__:
    mov al, 'K'
    out 0xe9, al

    lea rsp, [STACK_TOP]
    lea rbp, [STACK_TOP]

    mov al, 'S'
    out 0xe9, al

    call test

    mov al, 'E'
    out 0xe9, al
.dead:
    mov al, 'D'
    out 0xe9, al

    mov dil, 1
    call __kill__

    cli
    hlt
    jmp .dead


section .bss
align 4096
resb 4096*4
STACK_TOP: