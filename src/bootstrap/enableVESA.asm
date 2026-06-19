[bits 16]

VBE_GET_CONTROLLER_INFO         equ 0x4F00
VBE_GET_MODE_INFO               equ 0x4F01
VBE_SET_MODE                    equ 0x4F02
VBE_SUCCESS                     equ 0x004F

VBE_MODE_END                    equ 0xFFFF
VBE_REQUIRED_ATTRIBUTES         equ 0x0091
VBE_REQUIRED_BPP                equ 32
VBE_LINEAR_FRAMEBUFFER          equ 0x4000

VBE_INFO_SIG_OFFSET             equ 0x00
VBE_INFO_VIDEO_MODES            equ 0x0E

VBE_MODE_ATTR                   equ 0
VBE_MODE_PITCH                  equ 16
VBE_MODE_WIDTH                  equ 18
VBE_MODE_HEIGHT                 equ 20
VBE_MODE_BPP                    equ 25
VBE_MODE_LFB_ADDR               equ 40

; >> void
; << MapVESA* in ax
enable_VESA_start:
    pusha
    push                es

    xor                 ax,             ax
    mov                 es,             ax
    mov                 di,             VBA_INFO_BLOCK
    mov                 dword [es:di],  "VESA"
    
    mov                 ax,             VBE_GET_CONTROLLER_INFO
    int 0x10
    cmp                 ax,             VBE_SUCCESS
    jne .critical_error

    xor                 bp,             bp
    xor                 dx,             dx


    mov                 si,             [VBA_INFO_BLOCK + VBE_INFO_VIDEO_MODES]
    mov                 ax,             [VBA_INFO_BLOCK + VBE_INFO_VIDEO_MODES + 2]
    mov                 fs,             ax

.loop_modes:
    mov                 cx,             [fs:si]
    cmp                 cx,             VBE_MODE_END
    je .search_done

    push                cx
    xor                 ax,             ax
    mov                 es,             ax
    mov                 di,             VBE_MODE_INFO   
    
    mov                 ax,             VBE_GET_MODE_INFO
    int 0x10
    pop                 cx
    
    cmp                 ax,             VBE_SUCCESS
    jne                 .next_mode

    mov                 ax,             [VBE_MODE_INFO + VBE_MODE_ATTR]
    and                 ax,             VBE_REQUIRED_ATTRIBUTES
    cmp                 ax,             VBE_REQUIRED_ATTRIBUTES
    jne                 .next_mode

    mov                 al,             [VBE_MODE_INFO + VBE_MODE_BPP]
    cmp                 al,             VBE_REQUIRED_BPP
    jne .next_mode

    mov                 ax,             [VBE_MODE_INFO + VBE_MODE_WIDTH]
    mov                 bx,             [VBE_MODE_INFO + VBE_MODE_HEIGHT]

    cmp                 ax,             bp
    ja                  .new_winner
    jb                  .next_mode
    
    cmp                 bx,             dx
    jbe                 .next_mode

.new_winner:
    mov                 bp,             ax
    mov                 dx,             bx
    
    mov                 eax,            [VBE_MODE_INFO + VBE_MODE_LFB_ADDR]
    mov                 [KERNEL_GRAPHICS_INFO.LBF_ADDRESS],     eax
    
    mov                 [KERNEL_GRAPHICS_INFO.WIDTH],           bp
    mov                 [KERNEL_GRAPHICS_INFO.HEIGHT],          dx
    
    mov                 ax,             [VBE_MODE_INFO + VBE_MODE_PITCH]
    mov                 [KERNEL_GRAPHICS_INFO.PITCH],           ax
    
    mov                 [KERNEL_GRAPHICS_INFO.MODE_NUMBER],     cx

.next_mode:
    add                 si,             2
    jmp                 .loop_modes


.search_done:
    test                bp,             bp
    jz                  .critical_error

    mov                 ax,             VBE_SET_MODE
    mov                 bx,             [KERNEL_GRAPHICS_INFO.MODE_NUMBER]
    or                  bx,             VBE_LINEAR_FRAMEBUFFER
    int 0x10
    
    cmp                 ax,             VBE_SUCCESS
    jne                 .critical_error

    pop                 es
    popa
    jmp                 enable_VESA_done

.critical_error:
    cli
    hlt


align 4
VBA_INFO_BLOCK:    times 512 db 0
VBE_MODE_INFO:     times 256 db 0

KERNEL_GRAPHICS_INFO:
    .LBF_ADDRESS:   dd 0
    .WIDTH:         dw 0
    .HEIGHT:        dw 0
    .PITCH:         dw 0
    .MODE_NUMBER:   dw 0


enable_VESA_done: