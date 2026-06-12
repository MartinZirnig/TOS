[BITS 32]

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

SECTION .text
global __entry__
extern initializeAllocator
extern analyzeMapping
extern createPagingTable
extern VGA_Initialize
extern VGA_Clear
extern VGA_NewLine
extern printSuccess
extern printInfo
extern sleep

__entry__:
    mov esp, stack_top

    cdecl             VGA_Initialize,         VGA_BOTTOM,         VGA_LIMIT
    cdecl             printSuccess,           ENTRY_MESSAGE
    cdecl             sleep,                  1000
    cdecl             VGA_NewLine


    cdecl             printInfo,              ALLOCATOR_START
    cdecl             initializeAllocator
    cdecl             printSuccess,           ALLOCATOR_DONE
    cdecl             sleep,                  1000
    cdecl             VGA_NewLine

    cdecl             printInfo,              MAPPING_START
    cdecl             analyzeMapping
    cdecl             printSuccess,           MAPPING_DONE
    cdecl             sleep,                  1000
    cdecl             VGA_NewLine

    cdecl             printInfo,              PAGING_START
    cdecl             createPagingTable
    cdecl             printSuccess,           PAGING_DONE
    cdecl             sleep,                  1000
    cdecl             VGA_NewLine

    cli
    hlt





section .data
ENTRY_MESSAGE:              db 'Bootstrap active, stack initialized!', 0
ALLOCATOR_START:            db 'Allocator initialization started!', 0
ALLOCATOR_DONE:             db 'Allocator initialization completed!', 0  
MAPPING_START:              db 'Memory mappings analyzation started!', 0
MAPPING_DONE:               db 'Memory mappings analyzation completed!', 0
PAGING_START:               db 'Paging table initialization started!', 0
PAGING_DONE:                db 'Paging table initialization completed!', 0


section .bss
align 16
stack_bottom:
    resb 0xFF
stack_top: