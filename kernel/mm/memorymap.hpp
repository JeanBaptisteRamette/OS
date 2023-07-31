#ifndef OS_MEMORYMAP_HPP
#define OS_MEMORYMAP_HPP

#include <osl/mindef.hpp>
#include <limine.h>

struct KMEMORY_MAP_INFO
{
    QWORD UsableEntries;        // Count of usable entries
    QWORD TotalAvailable;       // Size of total available memory, in bytes
};


limine_memmap_response* KeGetMemoryMap();
void DbgDisplayMemoryMap();
QWORD KeGetMemoryMapInfo(KMEMORY_MAP_INFO& Info);

#endif //OS_MEMORYMAP_HPP
