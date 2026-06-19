%ifndef CONSTANTS_INC
%define CONSTANTS_INC

SEGMENTATION_16                         equ 0x20
SEGMENTATION_32                         equ 0x10
SEGMENTATION_64                         equ 0x30

LONG_JUMP                               equ 0x0000
LONG_JUMP_16                            equ 0x18
LONG_JUMP_32                            equ 0x08
LONG_JUMP_64                            equ 0x28

EXPECTED_KERNEL_ADDRESS                 equ 0xFFFFFFFF80000000

GDT_ACCESS_ACCESSED                     equ 1 << 0
GDT_ACCESS_READ_WRITE                   equ 1 << 1
GDT_ACCESS_DIRECTION_PROTECTION         equ 1 << 2
GDT_ACCESS_EXECUTABLE                   equ 1 << 3
GDT_ACCESS_DESCRIPTOR_TYPE              equ 1 << 4
GDT_ACCESS_PRIVILEGE_A                  equ 1 << 5
GDT_ACCESS_PRIVILEGE_B                  equ 1 << 6
GDT_ACCESS_PRESENT                      equ 1 << 7

GDT_FLAG_LIMIT_16                       equ 1 << 0
GDT_FLAG_LIMIT_17                       equ 1 << 1
GDT_FLAG_LIMIT_18                       equ 1 << 2
GDT_FLAG_LIMIT_19                       equ 1 << 3
GDT_FLAG_RESERVED                       equ 1 << 4
GDT_FLAG_LONG_MODE_ACTIVE               equ 1 << 5 ; (Bit L)
GDT_FLAG_PROTECTED_MODE_ACTIVE          equ 1 << 6 ; (Bit D/B)
GDT_FLAG_BYTE_OR_PAGE                   equ 1 << 7 ; (Bit G)

%endif