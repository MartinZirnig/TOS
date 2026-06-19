#include "bootstrap.h"
#include "mmap.h"
#include "paging.h"
#include "math.h"

#define MMAP_ADDRESS                        0x0500
#define LOW_PAGE_META                       PTE_Present | PTE_Writable
#define RESERVED_PAGE_META                  PTE_Present | PTE_Writable
#define KERNEL_PAGE_META                    PTE_Present | PTE_Writable
#define GENERAL_PDE_META                    PDE_Present | PDE_Writable

#define MAX_SUPPORTED_PAGING_TABLES         0x20


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
        
        fill((void*)current->pages, ENTRIES_PER_TABLE * sizeof(PageTableEntry), 0);
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


// Pomocná funkce, která vyčistí CELÝ záznam a nastaví jen to, co chceme (včetně adresy a flagů)
static inline void _writeTableEntry(safeptr(PageTableEntry) entry, qword physAddr, qword flags) {
    qword clean_addr = physAddr & ADDR_MASK;
    qword clean_flags = flags & META_MASK;
    *entry = clean_addr | clean_flags; // Žádné staré bity nepřežijí, bity 52-63 jsou 0!
}

static inline void _mapPT(Range_64 range, qword virtualStart, PTE access, safeptr(PagingTable) table) {
    qword vStart = virtualStart;
    while (range.bot < range.top) {
        word index = (word)((vStart & BIT64_ADDRESS_PT) >> 12);
        safeptr(PageTableEntry) pte = (table->pages + index);

        // OPRAVA: Pro 4KB stránky NESMÍ být PDE_HugePage!
        _writeTableEntry(pte, range.bot, access); 

        range.bot += NORMAL_PAGE_SIZE;
        vStart += NORMAL_PAGE_SIZE;
    }
}

static inline void _mapPD(Range_64 range, qword virtualStart, PTE access, safeptr(PagingTable) table) {       
    qword vStart = virtualStart;
    while (range.bot < range.top) {
        word index = (word)((vStart & BIT64_ADDRESS_PD) >> 21);
        safeptr(PageTableEntry) pte = (table->pages + index);
        
        qword vNextBoundary = (vStart & ~(HUGE_PAGE_SIZE - 1)) + HUGE_PAGE_SIZE;
        qword currentChunkEnd = min(range.top, range.bot + (vNextBoundary - vStart));
        Range_64 clear = { range.bot, currentChunkEnd };

        // Pokud mapujeme přesně celý 2MB blok a je zarovnaný, můžeme použít 2MB stránku
        if ((vStart & (HUGE_PAGE_SIZE - 1)) == 0 && (range.bot & (HUGE_PAGE_SIZE - 1)) == 0 && (currentChunkEnd - range.bot) == HUGE_PAGE_SIZE) {
            _writeTableEntry(pte, range.bot, access | PDE_HugePage); // Tady je HugePage legální
        } else {
            // Jinak musíme jít o úroveň níž do PT tabulky
            bool isSet = (*pte) & PTE_Present;
            qword nextTableAddress = isSet ? Paging_GetNextAddress(pte) : (qword)(nuint)_addTable();
            
            _mapPT(clear, vStart, access, (safeptr(PagingTable))(nuint)nextTableAddress);
            _writeTableEntry(pte, nextTableAddress, PDE_Present | PDE_Writable); // Mezistupeň! Bez HugePage!
        }

        qword mapped = currentChunkEnd - range.bot;
        range.bot += mapped;
        vStart += mapped;
    }
}

static inline void _mapPDPT(Range_64 range, qword virtualStart, PTE access, safeptr(PagingTable) table) {
    qword vStart = virtualStart;
    while (range.bot < range.top) {
        word index = (word)((vStart & BIT64_ADDRESS_PDPT) >> 30);
        safeptr(PageTableEntry) pte = (table->pages + index);
        
        bool isSet = (*pte) & PTE_Present;
        qword nextTableAddress = isSet ? Paging_GetNextAddress(pte) : (qword)(nuint)_addTable();

        qword vNextBoundary = (vStart & ~(GIANT_PAGE_SIZE - 1)) + GIANT_PAGE_SIZE;
        qword currentChunkEnd = min(range.top, range.bot + (vNextBoundary - vStart));
        Range_64 clear = { range.bot, currentChunkEnd };
  
        _mapPD(clear, vStart, access, (safeptr(PagingTable))(nuint)nextTableAddress);
        
        _writeTableEntry(pte, nextTableAddress, PDE_Present | PDE_Writable); // Mezistupeň! Bez HugePage!
    
        qword mapped = currentChunkEnd - range.bot;
        range.bot += mapped;
        vStart += mapped;
    }
}

static inline void _mapPML4(Range_64 range, qword virtualStart, PTE access) {
    qword vStart = virtualStart;
    while (range.bot < range.top) {
        word index = (word)((vStart & BIT64_ADDRESS_PML4) >> 39);
        safeptr(PageTableEntry) pte = (PML4->pages + index);
        
        bool isSet = (*pte) & PTE_Present;
        qword nextTableAddress = isSet ? Paging_GetNextAddress(pte) : (qword)(nuint)_addTable();

        qword vNextBoundary = (vStart & ~(PDPT_LIMIT - 1)) + PDPT_LIMIT;
        qword currentChunkEnd = min(range.top, range.bot + (vNextBoundary - vStart));
        Range_64 clear = { range.bot, currentChunkEnd };

        _mapPDPT(clear, vStart, access, (safeptr(PagingTable))(nuint)nextTableAddress);
        
        _writeTableEntry(pte, nextTableAddress, PDE_Present | PDE_Writable); // Mezistupeň! Bez HugePage!

        qword mapped = currentChunkEnd - range.bot;
        range.bot += mapped;
        vStart += mapped;
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
            PGI.kernelPhysical = range.bot;
            
            description = str_concat(description, "  [kernel ");
            description = str_concat(description, _describeRange(splited));
            description = str_concat(description, "]");
            
            kernelLoaded = true;
        }

        PGI.PML4 = (qword)(nuint)PML4;
        PGI.maps = (qword)MMAP_ADDRESS;

        print(description);
        freeAll();
    }

    return (nuint)&PGI;
}