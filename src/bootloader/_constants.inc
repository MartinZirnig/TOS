%ifndef CONSTANTS_INC
%define CONSTANTS_INC

; <========== GENERAL ==========>
NULL                        equ 0x0
STACK_TOP                   equ 0x7C00
BOOT_MAGIC                  equ 0xAA55
BOOT_MAGIC_SIZE             equ 0x2
TOTAL_SIZE                  equ 0x200

BOOTSTRAP_ADDRESS           equ 0x8000
BOOTSTRAP_SECTOR            equ 0x0000    
BOOTSTRAP_SIZE              equ 0x64

READ_DISK                   equ 0x42


; <========== ENABLE PROTECTED MODE ==========>
FAR_JUMP_PREFFIX                        equ 0x08
SEGMENTATION_ADDRESS                    equ 0x10
GDT_LIMIT                               equ 0xFFFF
GDT_BASE                                equ 0x00

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
GDT_FLAG_LONG_MODE_ACTIVE               equ 1 << 5
GDT_FLAG_PROTECTED_MODE_ACTIVE          equ 1 << 6
GDT_FLAG_BYTE_OR_PAGE                   equ 1 << 7



; <========== MAPPING ==========>
MAPPING_META_START          equ 0x0500
MAPPING_DATA_START          equ 0x0502

MMAP_BIOS_CALL              equ 0xE820
SMAP_CALL_MAGIC             equ 0x534D4150
MMAP_STRUCT_SIZE            equ 0x18


; <========== TEST CPU ==========>
EXTENDED_SUPPORT_CALL           equ 0x80000000           
SUPPORTS_64BIT_CALL             equ 0x80000001
SUPPORTS_64BIT_FLAG             equ 1 << 29


; <========== PRINT ==========>
PRINT_BIOS_CALL             equ 0x0E
%endif