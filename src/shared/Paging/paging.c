#include "paging.h"


static inline void _setAddress(safeptr(PageTableEntry) entry, qword physicalAddress) {
    PageTableEntry current_flags = *entry & META_MASK;
    PageTableEntry clean_address = (PageTableEntry)physicalAddress & ADDR_MASK;
    *entry = clean_address | current_flags;
}

static inline qword _getAddress(safeptr(PageTableEntry) entry) {
    volatile PageTableEntry result = *entry;
    result &= ADDR_MASK;
    return result;
}


void Paging_SetPhysicalAddress(safeptr(PageTableEntry) entry, qword physicalAddress) {
    if (!entry) return;

    _setAddress(entry, physicalAddress);
}
qword Paging_GetPhysicalAddress(safeptr(PageTableEntry) entry) {
    return _getAddress(entry);
}

void Paging_SetMetadataPTE(safeptr(PageTableEntry) entry, PTE metadata) {
    if (!entry) return;

    PageTableEntry current_address = *entry & ADDR_MASK;
    PageTableEntry clean_metadata = (PageTableEntry)metadata & META_MASK;
    *entry = current_address | clean_metadata;
}

void Paging_SetNextAddress(safeptr(PageTableEntry) entry, qword physicalAddress) {
    if (!entry) return;

    _setAddress(entry, physicalAddress);
}
qword Paging_GetNextAddress(safeptr(PageTableEntry) entry) {
    return _getAddress(entry);
}

void Paging_SetMetadataPDE(safeptr(PageTableEntry) entry, PDE metadata) {
    if (!entry) return;
    PageTableEntry current_address = *entry & ADDR_MASK;
    PageTableEntry clean_metadata = (PageTableEntry)metadata & META_MASK;
    *entry = current_address | clean_metadata;
}