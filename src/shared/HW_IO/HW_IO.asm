section .text

; <========== 64 BIT ==========>
%ifidn __OUTPUT_FORMAT__, elf64
[bits 64]

global __kill__
global __in8__
global __in16__
global __in32__
global __out8__
global __out16__
global __out32__

__kill__:
    mov al, 1
    out 0xf4, al
.dead:
    cli
    hlt
    jmp .dead

__in8__:
    mov dx, di
    in al, dx
    ret

__in16__:
    mov dx, di
    in ax, dx
    ret

__in32__:
    mov dx, di
    in eax, dx
    ret

__out8__:
    mov dx, di
    mov al, sil
    out dx, al
    ret

__out16__:
    mov dx, di
    mov ax, si
    out dx, ax
    ret

__out32__:
    mov dx, di
    mov eax, esi
    out dx, eax
    ret

; <========== 32 BIT ==========>
%elifidn __OUTPUT_FORMAT__, elf32
[bits 32]

global __kill__
global __in8__
global __in16__
global __out8__
global __out16__

__kill__:
    mov al, [esp + 4] 
    out 0xf4, al

__in8__:
    mov dx, [esp + 4]  ; 1. argument (port) je na adrese ESP + 4
    in al, dx
    ret

__in16__:
    mov dx, [esp + 4]  ; 1. argument (port)
    in ax, dx
    ret

__out8__:
    mov dx, [esp + 4]  ; 1. argument (port)
    mov al, [esp + 8]  ; 2. argument (data) je o 4 bajty dál
    out dx, al
    ret

__out16__:
    mov dx, [esp + 4]  ; 1. argument (port)
    mov ax, [esp + 8]  ; 2. argument (data)
    out dx, ax
    ret

%endif