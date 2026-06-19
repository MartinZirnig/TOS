[BITS 32]

%include "src/bootstrap/_constants.inc"

VGA_BOTTOM                  equ 0xB8000
VGA_LIMIT                   equ 0xB8FA0

%macro cdecl 1-*
    %assign argc %0 - 1
    %assign stack_size argc * 4

    %if argc > 0
        %rep argc
            %rotate -1
            push dword %1
        %endrep
    %endif
    %rotate -1
    call %1

    %if stack_size > 0
        add esp, stack_size
    %endif
%endmacro

section .text
global __entry__
extern initializeAllocator
extern analyzeMapping
extern createPagingTable
extern loadKernel
extern VGA_Initialize
extern VGA_Clear
extern VGA_NewLine
extern printSuccess
extern printInfo
extern sleep
extern __kill__

__entry__:
    cli
    mov                 esp,                      STACK_TOP

    cdecl               VGA_Initialize,           VGA_BOTTOM,         VGA_LIMIT
    %include "src/bootstrap/updateGDT.asm"
    cdecl               printSuccess,             ENTRY_MESSAGE
    cdecl               sleep,                    1000
    cdecl               VGA_NewLine



    cdecl               printInfo,                ALLOCATOR_START
    cdecl               initializeAllocator
    cdecl               printSuccess,             ALLOCATOR_DONE
    cdecl               sleep,                    1000
    cdecl               VGA_NewLine

    cdecl               printInfo,                MAPPING_START
    cdecl               analyzeMapping
    cdecl               printSuccess,             MAPPING_DONE
    cdecl               sleep,                    1000
    cdecl               VGA_NewLine

    cdecl               printInfo,                PAGING_START
    cdecl               createPagingTable
    mov                 [PAGING_INIT],            eax
    cdecl               printSuccess,             PAGING_DONE
    cdecl               sleep,                    1000
    cdecl               VGA_NewLine

    cdecl               printInfo,                LOADING_START
    mov                 eax,                      [PAGING_INIT]
    cdecl               loadKernel,               eax
    cdecl               sleep,                    1000
    cdecl               printSuccess,             LOADING_DONE
    cdecl               VGA_NewLine

    cdecl               printInfo,                DISABLE_VGA_MSG_1
    cdecl               sleep,                    500
    cdecl               printInfo,                DISABLE_VGA_MSG_2
    cdecl               sleep,                    500
    cdecl               printInfo,                DISABLE_VGA_MSG_3
    cdecl               sleep,                    500
    cdecl               printInfo,                DISABLE_VGA_MSG_4
    cdecl               sleep,                    500
    cdecl               printInfo,                DISABLE_VGA_MSG_5
    cdecl               sleep,                    500


    %include "src/bootstrap/swap_to_16.asm"
    [bits 16]
    %include "src/bootstrap/enableVESA.asm"
    mov                 [VESA_INIT],               ax

    %include "src/bootstrap/swap_to_32.asm"
    [bits 32]

    mov                 eax,                        [PAGING_INIT]
    mov                 eax,                        [eax]

    %include "src/bootstrap/swap_to_64.asm"
    [bits 64]


    mov                 R10,                    [PAGING_INIT]
    mov                 R11,                    [VESA_INIT]       
    mov                 rax,                    EXPECTED_KERNEL_ADDRESS
    jmp                 rax

section .rodata
ENTRY_MESSAGE:              db 'Bootstrap active, stack initialized, extended GDT enabled!', 0
ALLOCATOR_START:            db 'Allocator initialization started!', 0
ALLOCATOR_DONE:             db 'Allocator initialization completed!', 0  
MAPPING_START:              db 'Memory mappings analyzation started!', 0
MAPPING_DONE:               db 'Memory mappings analyzation completed!', 0
PAGING_START:               db 'Paging table initialization started!', 0
PAGING_DONE:                db 'Paging table initialization completed!', 0
LOADING_START:              db 'Kernel loading started!', 0
LOADING_DONE:               db 'Kernel loading completed!', 0
DISABLE_VGA_MSG_1:          db 'Disabling VGA and swaping to graphical output', 0
DISABLE_VGA_MSG_2:          db 'There will be no other logging until kernel is estabilished', 0
DISABLE_VGA_MSG_3:          db 'starts in 3', 0
DISABLE_VGA_MSG_4:          db 'starts in 2', 0
DISABLE_VGA_MSG_5:          db 'starts in 1', 0


align 16
STACK_BOTTOM:
    times 0xFFF db 1
STACK_TOP:

PAGING_INIT                 dd 0
VESA_INIT                   dw 0