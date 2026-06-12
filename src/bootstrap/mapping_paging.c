#include "bootstrap.h"
#include "mmap.h"
#include "paging.h"
#include "HW_IO.h"
#include "math.h"

#define MMAP_ADDRESS                        0x0500
#define LOW_PAGE_META                       PTE_Present | PTE_Writable | PTE_CancelPageCache | PTE_Global
#define RESERVED_PAGE_META                  PTE_Present | PTE_Writable | PTE_CancelPageCache
#define KERNEL_PAGE_META                    PTE_Present | PTE_Writable | PTE_CancelPageCache | PTE_Global
#define GENERAL_PDE_META                    PDE_Present | PDE_Writable | PDE_UseWriteCache | PDE_CancelPageCache | PDE_Global

#define MAX_SUPPORTED_MEMORY_SEGMENTS       0xFF
#define MAX_SUPPORTED_PAGING_TABLES         0x40


typedef struct
{
    uint64 bot;
    uint64 top;
} Range_64;

typedef struct
{
    qword PML4;
    qword maps;

    uint32 rangeCount;
    Range_64 availableRanges[MAX_SUPPORTED_MEMORY_SEGMENTS];
} PagingInit_64Bit;


static volatile PagingInit_64Bit PGI;
static volatile uint32 usedTables = 1;
static volatile PagingTable tables[MAX_SUPPORTED_PAGING_TABLES] __attribute__((aligned(NORMAL_PAGE_SIZE)));
static safeptr(PagingTable) PML4 = tables;

static inline void _addRange(qword start, qword end)
{
    PGI.availableRanges[PGI.rangeCount].bot = start;
    PGI.availableRanges[PGI.rangeCount].top = end;
    PGI.rangeCount += 1;
}


#define ALLIGN_UP                           true
#define ALLIGN_DOWN                         false              
static inline qword _alignAddress(qword address, bool up) {
    qword addr;
    
    if (up) {
        addr = div64(address, NORMAL_PAGE_SIZE) * NORMAL_PAGE_SIZE;
        if (mod64(address, NORMAL_PAGE_SIZE))
            addr += NORMAL_PAGE_SIZE;
        return addr;
    } else {
        addr = div64(address, NORMAL_PAGE_SIZE) * NORMAL_PAGE_SIZE;
    }

    return addr;
}

void analyzeMapping() {
    Mmaps* maps = (Mmaps*)MMAP_ADDRESS;

    printInfo("Searching for available memory pages...");
    for (uint32 i = 0; i < maps->size; i++) {
        Mmap map = maps->maps[i];
        if (map.type != MMAP_Available) continue;

        qword bot = _alignAddress(map.address, ALLIGN_UP);
        qword top = _alignAddress(map.address + map.length, ALLIGN_DOWN);

        _addRange(bot, top);

        str message = "    ";
        message = str_concat(message, uint64ToString(bot));
        message = str_concat(message, " -> ");
        message = str_concat(message, uint64ToString(top));

        print(message);
        freeAll();
    }
}


static inline void _initTables() {
    for (uint32 i = 0; i < MAX_SUPPORTED_PAGING_TABLES; i++) {
        safeptr(PagingTable) current = (tables + i);
        
        fill((void*)current->pages, ENTRIES_PER_TABLE, UINT64_MIN);
    }
}

static inline Range_64 _getCorectRange(Mmap* map) {
    Range_64 result;

    if (map->type == MMAP_Available) {
        qword bot = _alignAddress(map->address, ALLIGN_UP);
        qword top = _alignAddress(map->address + map->length, ALLIGN_DOWN);
    
        result.top = top;
        result.bot = bot;
    } else {
        qword bot = _alignAddress(map->address, ALLIGN_DOWN);
        qword top = _alignAddress(map->address + map->length, ALLIGN_UP);

        result.top = top;
        result.bot = bot;
    }

    return result;
}

static inline safeptr(PagingTable) _addTable() {
    if (usedTables >= MAX_SUPPORTED_PAGING_TABLES) {
        printError("Cannot ensure enought mapping for reserved memory!");
        sleep(1500);
        __kill__(101);
    }
    
    safeptr(PagingTable) ptr = (tables + usedTables);
    usedTables += 1;
    return ptr; 
}

static void _throwMemoryAllreadyMappedException() {
    printError("One memory segment reserved by multiple pages!");
    sleep(1500);
    __kill__(101);
}


static inline void _mapPT(Range_64 range, qword virtualStart, PTE access, safeptr(PagingTable) table) {
    str msg = uint64ToString((range.top - range.bot) / NORMAL_PAGE_SIZE); 
    print(msg);
    free(msg);

    while (range.top - range.bot >= NORMAL_PAGE_SIZE) {
        word index = (word)((range.bot & BIT64_ADDRESS_PT) >> 12);

        safeptr(PageTableEntry) pte = (table->pages + index);
        
        if ((*pte) & PTE_Present) 
            _throwMemoryAllreadyMappedException();

        Paging_SetMetadataPTE(pte, access | PDE_HugePage);
        Paging_SetNextAddress(pte, virtualStart);

        range.bot += NORMAL_PAGE_SIZE;
        virtualStart += NORMAL_PAGE_SIZE;
    }
}

static inline void _mapPD(Range_64 range, qword virtualStart, PTE access, safeptr(PagingTable) table) {   
    printError("pd");
    
    while (range.top - range.bot >= HUGE_PAGE_SIZE) {
        printError("pd__");
        word index = (word)((range.bot & BIT64_ADDRESS_PD) >> 21);

        safeptr(PageTableEntry) pte = (table->pages + index);
        if ((*pte) & PTE_Present) 
            _throwMemoryAllreadyMappedException();

        Paging_SetMetadataPTE(pte, access | PDE_HugePage);
        Paging_SetNextAddress(pte, virtualStart);

        range.bot += HUGE_PAGE_SIZE;
        virtualStart += HUGE_PAGE_SIZE;
    }

    if (range.bot < range.top) {
        word index = (word)((range.bot & BIT64_ADDRESS_PD) >> 21);

        safeptr(PageTableEntry) pte = (table->pages + index);
        bool isSet = (*pte) & PTE_Present;
        
        qword nextTableAddress = isSet
            ? Paging_GetNextAddress(pte)
            : (qword)(nuint)_addTable();

        _mapPT(range, virtualStart, access, (safeptr(PagingTable))(nuint)nextTableAddress);
        Paging_SetNextAddress(pte, nextTableAddress);
        Paging_SetMetadataPDE(pte, GENERAL_PDE_META);
    }
}

static inline void _mapPDPT(Range_64 range, qword virtualStart, PTE access, safeptr(PagingTable) table) {
    printError("pdpt");

    while (range.top - range.bot >= GIANT_PAGE_SIZE) {
        printError("pdpt__");
        
        word index = (word)((range.bot & BIT64_ADDRESS_PDPT) >> 30);

        safeptr(PageTableEntry) pte = (table->pages + index);
        if ((*pte) & PTE_Present) 
            _throwMemoryAllreadyMappedException();

        Paging_SetMetadataPTE(pte, access | PDE_HugePage);
        Paging_SetNextAddress(pte, virtualStart);

        range.bot += GIANT_PAGE_SIZE;
        virtualStart += GIANT_PAGE_SIZE;
    }

    if (range.bot < range.top) {
        word index = (word)((range.bot & BIT64_ADDRESS_PDPT) >> 30);


        safeptr(PageTableEntry) pte = (table->pages + index);
        bool isSet = (*pte) & PTE_Present;
        
        qword nextTableAddress = isSet
            ? Paging_GetNextAddress(pte)
            : (qword)(nuint)_addTable();

  
        _mapPD(range, virtualStart, access, (safeptr(PagingTable))(nuint)nextTableAddress);
        Paging_SetNextAddress(pte, nextTableAddress);
        Paging_SetMetadataPDE(pte, GENERAL_PDE_META);
    }
}

static inline void _mapPML4(Range_64 range, qword virtualStart, PTE access) {
    printError("pml4");
    while (range.bot < range.top) {
        print(uint64ToString(range.bot));

        word index = (word)((range.bot & BIT64_ADDRESS_PML4) >> 39);

        safeptr(PageTableEntry) pte = (PML4->pages + index);
        bool isSet = (*pte) & PTE_Present;
        
        qword nextTableAddress = isSet
            ? Paging_GetNextAddress(pte)
            : (qword)(nuint)_addTable();

        Range_64 clear = { range.bot, min(range.top, range.bot + PDPT_LIMIT) };

        _mapPDPT(clear, virtualStart, access, (safeptr(PagingTable))(nuint)nextTableAddress);
        Paging_SetNextAddress(pte, nextTableAddress);
        Paging_SetMetadataPDE(pte, GENERAL_PDE_META);

        range.bot += PDPT_LIMIT;
        virtualStart += PDPT_LIMIT;
    }
}

static inline str _describeRange(Range_64 range) {
    str bot = uint64ToString(range.bot);
    str top = uint64ToString(range.top);

    str a = str_concat(bot, " -> ");
    str b = str_concat(a, top);

    free(bot);
    free(top);
    free(a);

    return b;
}

static inline str _describeMmapType(dword type) {
    switch (type)
    {
    case MMAP_Available:
        return "Available: ";
    case MMAP_Reserved:
        return "Reserved: ";
    case MMAP_ACPI_Tmp:
        return "ACPI_temp: ";
    case MMAP_ACPI_NVS:
        return "ACPI_NVS: ";
    case MMAP_Unusable:
        return "Unusabele: ";
    default:
        return "Unknown: ";
    }
}

static inline void _fixKernelMap(Range_64 range) {
    for (uint32 i = 0; i < PGI.rangeCount; i++) {
        if (PGI.availableRanges[i].bot == range.bot) {
            PGI.availableRanges[i].bot += EXPECTED_KERNEL_SIZE;
            return;
        }
    }

    printError("Cannot fix kernel address, no analyzed range found!");
    __kill__(101);
}

nuint createPagingTable() {
    printInfo("Clearing paging tables...");
    _initTables();

    printInfo("Initializing paging tables mappings...");
    Mmaps* maps = (Mmaps*)MMAP_ADDRESS;

    bool kernelLoaded = false;

    for (uint32 i = 0; i < maps->size; i++) {
        Mmap map = maps->maps[i];

        Range_64 defaultRange = { map.address, map.address + map.length };
        Range_64 range = _getCorectRange(&map);
        uint64 actualSize = range.top - range.bot;

        str description = "    ";
        description = str_concat(description, _describeMmapType(map.type));
        description = str_concat(description, _describeRange(defaultRange));

        if (map.address == 0) {
            _mapPML4(range, 0, LOW_PAGE_META);
            
            description = str_concat(description, "  [low]");
        } else if (map.type != MMAP_Available) {
            _mapPML4(range, range.bot, RESERVED_PAGE_META);
            
            description = str_concat(description, "  [reserved ");
            description = str_concat(description, _describeRange(range));
            description = str_concat(description, "]");
        } else if (actualSize < EXPECTED_KERNEL_SIZE) {
            description = str_concat(description, "  [skip (to small)]");
        } else if (kernelLoaded) {
            description = str_concat(description, "  [skip (kernel allready mapped)]");
        } else {
            _fixKernelMap(range);
            Range_64 splited = { range.bot, range.bot + EXPECTED_KERNEL_SIZE };
            _mapPML4(splited, EXPECTED_KERNEL_ADDRESS, KERNEL_PAGE_META);
            
            description = str_concat(description, "  [kernel ");
            description = str_concat(description, _describeRange(splited));
            description = str_concat(description, "]");
            kernelLoaded = true;
        }


        print(description);
        freeAll();
    }

    return (nuint)&PGI;
}