#ifndef PAGING_H
#define PAGING_H

#include "std.h"
#include "mmap.h"

#define ENTRIES_PER_TABLE           512ULL

#define NORMAL_PAGE_SIZE            4096ULL
#define HUGE_PAGE_SIZE              (NORMAL_PAGE_SIZE * ENTRIES_PER_TABLE)
#define GIANT_PAGE_SIZE             (HUGE_PAGE_SIZE * ENTRIES_PER_TABLE)
#define PDPT_LIMIT                  (GIANT_PAGE_SIZE * ENTRIES_PER_TABLE)
#define ADDR_MASK                   0x000FFFFFFFFFF000ULL
#define META_MASK                   0x0000000000000FFFULL

typedef enum 
{
    PDE_Present             = 1ULL << 0,
    PDE_Writable            = 1ULL << 1,
    PDE_UserAccess          = 1ULL << 2,
    PDE_UseWriteCache       = 1ULL << 3,
    PDE_CancelPageCache     = 1ULL << 4,
    PDE_Accessed            = 1ULL << 5,
    PDE_Reserved            = 1ULL << 6,
    PDE_HugePage            = 1ULL << 7,
    PDE_Global              = 1ULL << 8,
    PDE_Meta_1              = 1ULL << 9,
    PDE_Meta_2              = 1ULL << 10,
    PDE_Meta_3              = 1ULL << 11,
    PDE_Meta                = (
        PDE_Meta_1 | 
        PDE_Meta_2 | 
        PDE_Meta_3
    ),
    PDE_Address             = ADDR_MASK
} PDE;

typedef enum 
{
    PTE_Present             = 1ULL << 0,
    PTE_Writable            = 1ULL << 1,
    PTE_UserAccess          = 1ULL << 2,
    PTE_UseWriteCache       = 1ULL << 3,
    PTE_CancelPageCache     = 1ULL << 4,
    PTE_Accessed            = 1ULL << 5,
    PTE_Written             = 1ULL << 6,
    PTE_Reserved            = 1ULL << 7,
    PTE_Global              = 1ULL << 8,
    PTE_Meta_1              = 1ULL << 9,
    PTE_Meta_2              = 1ULL << 10,
    PTE_Meta_3              = 1ULL << 11,
    PTE_Meta                = (
        PTE_Meta_1 | 
        PTE_Meta_2 | 
        PTE_Meta_3
    ),
    PTE_Address             = ADDR_MASK
} PTE;

typedef qword PageTableEntry;

typedef struct __attribute__((aligned(NORMAL_PAGE_SIZE)))
{
    PageTableEntry pages[ENTRIES_PER_TABLE];
} PagingTable;

typedef struct 
{
    void* kernelPhysical;
    PagingTable* PML4;
    Mmaps* maps;
    
    uint32 rangeCount;
    Range availableRanges[];
} PagingInit;



#ifdef __cplusplus
extern "C" {
#endif

void Paging_SetPagingTable(safeptr(PagingTable) table);

void Paging_SetPhysicalAddress(safeptr(PageTableEntry) entry, qword physicalAddress);
qword Paging_GetPhysicalAddress(safeptr(PageTableEntry) entry);
void Paging_SetMetadataPTE(safeptr(PageTableEntry) entry, PTE metadata);
void Paging_SetNextAddress(safeptr(PageTableEntry) entry, qword physicalAddress);
qword Paging_GetNextAddress(safeptr(PageTableEntry) entry);
void Paging_SetMetadataPDE(safeptr(PageTableEntry) entry, PDE metadata);

#ifdef __cplusplus
}
#endif

#endif