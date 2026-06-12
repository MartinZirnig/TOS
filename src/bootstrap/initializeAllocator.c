#include "bootstrap.h"
#include "HW_IO.h"

#define BUFFER_SIZE     0xFFF
#define NORMAL_PAGE_SIZE       0x20

static volatile byte buffer[BUFFER_SIZE];


void initializeAllocator() {
    void* start = (void*)buffer;
    void* end = (void*)((nuint)buffer + BUFFER_SIZE);
    
    printInfo("Creating allocator...");
    safeptr(Allocator) result = __create_allocator__(start, end, NORMAL_PAGE_SIZE);
    
    if (!result) {
        printError("Failed to create allocator!");
        __kill__(1);
    }
    
    printInfo("Setting shared allocator...");
    __setSharedAllocator__(result);
}