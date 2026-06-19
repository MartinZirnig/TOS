#include "allocator_impl.h"

static volatile Allocator* shared = null;

typedef struct Page {
    uint16 usedPages;
    int8 data[];
} Page;


static inline bool _isInRange(void* ptr, safeptr(Allocator) allocator) {
    return 
        (nuint)ptr >= (nuint)allocator->start && 
        (nuint)ptr <= (nuint)allocator->end;
}

static inline Page* _findUpperPageSegment(void* ptr, safeptr(Allocator) allocator) {
    if (!_isInRange(ptr, allocator))
        return null;

    nuint difference = (nuint)ptr - (nuint)allocator->start;
    nuint result = (nuint)allocator->start + difference + allocator->pageSize - (difference % allocator->pageSize);
    return (Page*)result;
}
static inline Page* _findLowerPageSegment(void* ptr, safeptr(Allocator) allocator) {
    if (!_isInRange(ptr, allocator))
        return null;

    nuint difference = (nuint)ptr - (nuint)allocator->start;
    nuint result = (nuint)allocator->start + difference - (difference % allocator->pageSize);
    return (Page*)result;
}

static inline Page* _findPageSegmentStart(void* position, safeptr(Allocator) allocator) {
    if (!_isInRange(position, allocator))
        return null;

    Page* base = allocator->start;
    while ((nuint)base < (nuint)allocator->end) {
        if (base->usedPages > 0) {
            nuint blockStart = (nuint)base;
            nuint blockEnd = blockStart + base->usedPages * allocator->pageSize;
            if ((nuint)position >= blockStart && (nuint)position < blockEnd) {
                return base;
            }
            base = (Page*)((nuint)base + base->usedPages * allocator->pageSize);
        } else {
            base += allocator->pageSize;
        }
    }
    return null;
}

static inline void* _allocate(uint32 size, safeptr(Allocator) allocator) {
    uint32 totalPages = (size + 2) / allocator->pageSize;
    if ((size + 2) % allocator->pageSize) totalPages++;

    nuint numericLimit = (nuint)allocator->end;
    Page* current = allocator->start;
    Page* allocationStart = allocator->start;
    uint32 emptyCount = 0;

    while ((nuint)current < numericLimit) {
        if (current->usedPages) {
            current = (void*)((nuint)current + current->usedPages * allocator->pageSize);
            allocationStart = null;
            emptyCount = 0; 
        } else {
            if (!allocationStart)
                allocationStart = current;
            emptyCount += 1;
            current = (void*)((nuint)current + allocator->pageSize);
        }

        if (emptyCount >= totalPages) {
            allocationStart->usedPages = totalPages;
            return (void*)allocationStart->data;
        }
    }
    return null;
}

static inline bool _reserve(void* address, uint32 size, safeptr(Allocator) allocator) {
    Page* lower = _findLowerPageSegment(address, allocator);
    if (!lower) return false;

    if (lower->usedPages)
        return false;

    Page* top = _findUpperPageSegment(((byte*)address + size), allocator);
    if ((nuint)top <= (nuint)lower) 
        return false;
    uint32 totalPages = ((nuint)top - (nuint)lower) / allocator->pageSize;

    for (uint32 i = 0; i < totalPages; i++) {
        if((lower + i)->usedPages)
            return false;
    }

    lower->usedPages = totalPages;
    return true;
}

bool _deallocate(void* address, safeptr(Allocator) allocator) {
    Page* start = _findPageSegmentStart(address, allocator);
    if (!start) return false;

    uint32 usedPages = start->usedPages;
    for (uint32 i = 0; i < usedPages; i++) {
        ((Page*)(((byte*)start) + i * allocator->pageSize))->usedPages = 0;
    }

    return true;
}

void _deallocateAll(safeptr(Allocator) allocator){
    Page* page = (Page*)allocator->start;

    while ((nuint)page <= (nuint)allocator->end) {
        page->usedPages = 0;

        page = (void*)((nuint)page + allocator->pageSize);
    }
}




safeptr(Allocator) __initialize_allocator__(void* start, void* end, void* memory, uint32 pageSize) {
    safeptr(Allocator) allocator = (safeptr(Allocator))memory;

    allocator->start = start;
    allocator->end = end;
    allocator->pageSize = pageSize;
    allocator->lock.isLocked = false;

    _deallocateAll(allocator);

    return allocator;
}

safeptr(Allocator) __create_allocator__(void* start, void* end, uint32 pageSize) {
    void* bufferStart = (void*)((nuint)start + sizeof(Allocator));
    return __initialize_allocator__(bufferStart, end, start, pageSize);
}


void* __malloc__(uint32 size, safeptr(Allocator) allocator) {
    __lock__(&allocator->lock);
    void* result = _allocate(size, allocator);
    __unlock__(&allocator->lock);
    
    return result;
}

void* __calloc__(uint32 size, uint32 count, safeptr(Allocator) allocator) {
    __lock__(&allocator->lock);
    void* result = _allocate(size * count, allocator);
    __unlock__(&allocator->lock);
    
    return result;
}

bool __reserve__(void* address, uint32 size, safeptr(Allocator) allocator) {
    __lock__(&allocator->lock);
    bool result = _reserve(address, size, allocator);
    __unlock__(&allocator->lock);
    
    return result;
}

bool __free__(void* address, safeptr(Allocator) allocator) {
    __lock__(&allocator->lock);
    bool result = _deallocate(address, allocator);
    __unlock__(&allocator->lock);
    
    return result;
}

void __freeAll__(safeptr(Allocator) allocator){
    __lock__(&allocator->lock);
    _deallocateAll(allocator);
    __unlock__(&allocator->lock);
}



void __setSharedAllocator__(safeptr(Allocator) allocator) {
    if (shared) {
        __lock__(&shared->lock);
        shared = allocator;
        __unlock__(&shared->lock);
    } else {
        shared = allocator;
    }
}

void* malloc(uint32 size) {
    if (shared) {
        __lock__(&shared->lock);
        void* result = _allocate(size, shared);
        __unlock__(&shared->lock);
    
        return result;
    }
    return null;
}

void* calloc(uint32 size, uint32 count) {
    if (shared) {
        __lock__(&shared->lock);
        void* result = _allocate(size * count, shared);
        __unlock__(&shared->lock);
    
        return result;
    }
    return null;
}

bool reserve(void* address, uint32 size) {
    if (shared) {
        __lock__(&shared->lock);
        bool result = _reserve(address, size, shared);
        __unlock__(&shared->lock);
    
        return result;
    }
    return false;
}

bool free(void* address){
    if (shared) {
        __lock__(&shared->lock);
        bool result = _deallocate(address, shared);
        __unlock__(&shared->lock);
    
        return result;
    }
    return false;
}

void freeAll(){
    if (shared) {
        __lock__(&shared->lock);
        _deallocateAll(shared);
        __unlock__(&shared->lock);
    }
}