#ifndef OS_LOCALBM_HPP
#define OS_LOCALBM_HPP

#include <std/mindef.hpp>

// To avoid using too much memory for the bitmap, we can use a per-region (local) bitmap.
// With a regular bitmap, the bitmap will keep state (used/unused) of unusable
// memory regions too. We only want to track the state of usable regions
// ie: LIMINE_MEMMAP_USABLE, but they are not adjacent in physical memory.


enum PAGE_STATE
{
    Unused = 0,
    Used   = 1
};

struct BITMAP_ENTRY
{
    QWORD RegionBase;
    QWORD RegionSize;
    QWORD BitIndex;     // Index corresponding to the first page of the region in the bitmap
};

struct BITMAP_LIST
{
    BITMAP_ENTRY* Entries;     // Array of regions
    QWORD BmCount;             // Amount of BITMAP_ENTRY in Entries
    PBYTE BmBase;              // Pointer to the whole bitmap
    QWORD BmSize;              // Size in bytes of the whole bitmap
};

void BmInit();
QWORD BmRequestPages(QWORD PageCount);
void BmReleasePages(QWORD Address, QWORD PageCount);

#endif //OS_LOCALBM_HPP
