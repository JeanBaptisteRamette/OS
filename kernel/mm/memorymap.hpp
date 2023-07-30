#ifndef OS_MEMORYMAP_HPP
#define OS_MEMORYMAP_HPP

#include <std/mindef.hpp>
#include <limine.h>


limine_memmap_response* KeGetMemoryMap();
void DbgDisplayMemoryMap();
QWORD KeMemMapGetUsableMemory();

#endif //OS_MEMORYMAP_HPP
