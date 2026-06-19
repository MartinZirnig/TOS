section .text

; <========== 64 BIT ==========>
%ifidn __OUTPUT_FORMAT__, elf64
[bits 64]

global __kill__
global __in8__
global __in16__
global __in32__
global __ins8__
global __ins16__
global __ins32__
global __rep_ins8__
global __rep_ins16__
global __rep_ins32__
global __out8__
global __out16__
global __out32__
global __outs8__
global __outs16__
global __outs32__
global __rep_outs8__
global __rep_outs16__
global __rep_outs32__

__kill__:
    mov al, dil
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

__ins8__:
    mov dx, di           ; RDI = port
    mov rdi, rsi         ; RSI = destination (přesuneme do RDI, jak vyžaduje insb)
    insb
    ret

__ins16__:
    mov dx, di           ; RDI = port
    mov rdi, rsi         ; RSI = destination
    insw
    ret

__ins32__:
    mov dx, di           ; RDI = port
    mov rdi, rsi         ; RSI = destination
    insd
    ret

__rep_ins8__:
    mov dx, di           ; RDI = port
    mov rax, rsi         ; RSI = destination
    mov rcx, rdx         ; RDX = times (přesuneme do RCX pro rep)
    mov rdi, rax         ; Cíl musí být v RDI
    rep insb
    ret

__rep_ins16__:
    mov dx, di           ; RDI = port
    mov rax, rsi         ; RSI = destination
    mov rcx, rdx         ; RDX = times
    mov rdi, rax
    rep insw
    ret

__rep_ins32__:
    mov dx, di           ; RDI = port
    mov rax, rsi         ; RSI = destination
    mov rcx, rdx         ; RDX = times
    mov rdi, rax
    rep insd
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

__outs8__:
    mov dx, di           ; RDI = port
    ; Zdroj je v RSI, což přesně odpovídá požadavku outsb
    outsb
    ret

__outs16__:
    mov dx, di           ; RDI = port
    outsw
    ret

__outs32__:
    mov dx, di           ; RDI = port
    outsd
    ret

__rep_outs8__:
    mov dx, di           ; RDI = port
    ; RSI už obsahuje src
    mov rcx, rdx         ; RDX = times -> do RCX
    rep outsb
    ret

__rep_outs16__:
    mov dx, di           ; RDI = port
    mov rcx, rdx         ; RDX = times -> do RCX
    rep outsw
    ret

__rep_outs32__:
    mov dx, di           ; RDI = port
    mov rcx, rdx         ; RDX = times -> do RCX
    rep outsd
    ret


; <========== 32 BIT ==========>
%elifidn __OUTPUT_FORMAT__, elf32
[bits 32]

global __kill__
global __in8__
global __in16__
global __ins8__
global __ins16__
global __rep_ins8__
global __rep_ins16__
global __out8__
global __out16__
global __outs8__
global __outs16__
global __rep_outs8__
global __rep_outs16__

__kill__:
    mov al, [esp + 4] 
    out 0xf4, al
.dead:
    cli
    hlt
    jmp .dead

__in8__:
    mov dx, [esp + 4]
    in al, dx
    ret

__in16__:
    mov dx, [esp + 4]
    in ax, dx
    ret

__ins8__:
    push edi             ; EDI musíme zálohovat (je to callee-saved registr)
    mov dx, [esp + 8]    ; port (+4 kvůli push edi)
    mov edi, [esp + 12]  ; destination
    insb
    pop edi              ; obnovit EDI
    ret

__ins16__:
    push edi
    mov dx, [esp + 8]    ; port
    mov edi, [esp + 12]  ; destination
    insw
    pop edi
    ret

__rep_ins8__:
    push edi
    push ecx             ; ECX raději taky zálohujeme
    mov dx, [esp + 12]   ; port (+8 kvůli dvěma pushům)
    mov edi, [esp + 16]  ; destination
    mov ecx, [esp + 20]  ; times
    rep insb
    pop ecx
    pop edi
    ret

__rep_ins16__:
    push edi
    push ecx
    mov dx, [esp + 12]   ; port
    mov edi, [esp + 16]  ; destination
    mov ecx, [esp + 20]  ; times
    rep insw
    pop ecx
    pop edi
    ret

__out8__:
    mov dx, [esp + 4]
    mov al, [esp + 8]
    out dx, al
    ret

__out16__:
    mov dx, [esp + 4]
    mov ax, [esp + 8]
    out dx, ax
    ret

__outs8__:
    push esi             ; ESI je pro outsb klíčový a musí se zálohovat
    mov dx, [esp + 8]    ; port (+4 kvůli push)
    mov esi, [esp + 12]  ; src
    outsb
    pop esi
    ret

__outs16__:
    push esi
    mov dx, [esp + 8]    ; port
    mov esi, [esp + 12]  ; src
    outsw
    pop esi
    ret

__rep_outs8__:
    push esi
    push ecx
    mov dx, [esp + 12]   ; port (+8 kvůli pushům)
    mov esi, [esp + 16]  ; src
    mov ecx, [esp + 20]  ; times
    rep outsb
    pop ecx
    pop esi
    ret

__rep_outs16__:
    push esi
    push ecx
    mov dx, [esp + 12]   ; port
    mov esi, [esp + 16]  ; src
    mov ecx, [esp + 20]  ; times
    rep outsw
    pop ecx
    pop esi
    ret

%endif