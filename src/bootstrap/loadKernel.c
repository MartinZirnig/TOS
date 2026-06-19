#include "bootstrap.h"
#include "math.h"

#define EXPECTED_KERNEL_DISK_ADDRESS        64 
#define KERNEL_SECTORS_COUNT                (EXPECTED_KERNEL_SIZE / DISK_SECTOR_SIZE)

#define SELECT_DISK_PORT                    0x1F6
#define DISK_MASK                           0xE0
#define SELECT_SECTOR_COUNT_PORT            0x1F2
#define LBA_LOW_PORT                        0x1F3
#define LBA_MID_PORT                        0x1F4
#define LBA_TOP_PORT                        0x1F5
#define READ_PORT                           0x1F7
#define READ_COMMAND                        0x20
#define BUSY_FLAG                           0x80
#define DATA_READY_FLAG                     0x08
#define DISK_DATA_PORT                      0x1F0


void _readSectors(byte count, void* destination, uint32 lba) {
    __out8__(SELECT_DISK_PORT, (byte)((lba >> 24) | DISK_MASK));
    __out8__(SELECT_SECTOR_COUNT_PORT, count);
    
    __out8__(LBA_LOW_PORT, (byte)(lba));
    __out8__(LBA_MID_PORT, (byte)((lba >> 8)));
    __out8__(LBA_TOP_PORT, (byte)((lba >> 16)));
    
    __out8__(READ_PORT, READ_COMMAND);

    while (count-- > 0) {
        while (__in8__(READ_PORT) & BUSY_FLAG)
            __asm__ volatile ("pause");
        while (!(__in8__(READ_PORT) & DATA_READY_FLAG))
            __asm__ volatile ("pause");
        
        __rep_ins16__(DISK_DATA_PORT, destination, 256);
        destination = (void*)((nuint)destination + 256 * sizeof(word));
    }
}

void loadKernel(PagingInit_64Bit* init) {
    uint32 lba = EXPECTED_KERNEL_DISK_ADDRESS;
    byte* target = (byte*)(nuint)init->kernelPhysical;
    uint32 sectorsLeft = KERNEL_SECTORS_COUNT;

    str initMsg = "Loading kernel sectors to physical address ";
    initMsg = str_concat(initMsg, itos((uint32)target));
    initMsg = str_concat(initMsg, "...");
    printInfo(initMsg);
    freeAll();

    while (sectorsLeft > 0) {
        byte readIteration = (byte)min(sectorsLeft, UINT8_MAX);
        _readSectors(readIteration, target, lba);

        str msg = "    loaded ";
        msg = str_concat(msg, itos((uint32)(readIteration * DISK_SECTOR_SIZE)));
        msg = str_concat(msg, " bytes from sector ");
        msg = str_concat(msg, itos(lba));

        target += (readIteration * DISK_SECTOR_SIZE);
        lba += readIteration;
        sectorsLeft -= readIteration;

        print(msg);
        freeAll();
    }
}