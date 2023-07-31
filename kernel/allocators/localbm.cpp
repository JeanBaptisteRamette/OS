#include <allocators/localbm.hpp>
#include <mm/memorymap.hpp>
#include <osl/printf.hpp>
#include <mm/common.hpp>
#include <osl/span.hpp>
#include <panic.hpp>


// Limine memory map notes:
// The entries are guaranteed to be sorted by base address, lowest to highest.
// Usable and bootloader reclaimable entries are guaranteed to be 4096 byte aligned for both base and length.
// Usable and bootloader reclaimable entries are guaranteed not to overlap with any other entry.
// To the contrary, all non-usable entries (including kernel/modules) are not guaranteed any alignment,
// nor is it guaranteed that they do not overlap other entries.


#define REGION_COUNT_PAGE(BmEntry) ((BmEntry.RegionSize) / (PAGE_SIZE))



//
// Global bitmap instance
//
static BITMAP_LIST Bitmap {
    .Entries = nullptr,
    .BmCount = 0,
    .BmBase  = nullptr,
    .BmSize  = 0,
};


static void BmResetBits()
{
    //
    // Sets all pages to unused
    //
    ZeroMemory(Bitmap.BmBase, Bitmap.BmSize);
}

static void BmLoadMemoryMap()
{
    const auto MemoryMap = KeGetMemoryMap();

    QWORD BmEntryIndex = 0;
    QWORD PageCount = 0;

    for (const auto* MmEntry : osl::span(MemoryMap->entries, MemoryMap->entry_count))
    {
        if (!MmEntry || MmEntry->type != LIMINE_MEMMAP_USABLE)
            continue;

        if (BmEntryIndex < Bitmap.BmCount)
        {
            Bitmap.Entries[BmEntryIndex].RegionBase = MmEntry->base;
            Bitmap.Entries[BmEntryIndex].RegionSize = MmEntry->length;
            Bitmap.Entries[BmEntryIndex].BitIndex   = PageCount;
        }

        ++BmEntryIndex;
        PageCount += MmEntry->length / PAGE_SIZE;
    }
}

static QWORD BmSizeFor(QWORD PageCount)
{
    // Computes the amount of bytes to allocate for the bitmap
    // 1 page = 1 bit
    // 8 page = 1 byte
    // 9 page = 2 byte
    // N page = (N / 8) + ((N % 8) != 0) byte
    //
    // PageCount >> 3 == PageCount / 8
    // PageCount &  7 == PageCount % 8
    //
    return (PageCount >> 3) + ((PageCount & 0b111) != 0);
}

static LPVOID BmEarlyAlloc(QWORD RequiredSize)
{
    //
    // Only use this function to allocate the bitmap structure ! This is not for page allocation
    //

    const auto MemoryMap = KeGetMemoryMap();

    //
    // Find a region in physical memory big enough to hold RequiredSize bytes
    //
    for (auto* Entry : osl::span(MemoryMap->entries, MemoryMap->entry_count))
    {
        if (Entry && Entry->type == LIMINE_MEMMAP_USABLE && Entry->length >= RequiredSize)
        {
            const auto Addr = Entry->base;

            // Adjust region's boundaries, so we are sure to not allocate at the same place twice
            // This also means we do not need to set the bitmap pages as used
            // As of Limine's protocol, base and length of a usable region should always be page aligned
            // The length is at least 4096 bytes, so it cannot under-flow
            Entry->base   += PAGE_ALIGN_UP(RequiredSize);
            Entry->length -= PAGE_ALIGN_UP(RequiredSize);

            return reinterpret_cast<LPVOID>(Addr);
        }
    }

    return nullptr;
}

static QWORD BmBitIndexFromAddress(QWORD Address)
{
    //
    // Address is page aligned
    //

    for (const BITMAP_ENTRY& BmEntry : osl::span(Bitmap.Entries, Bitmap.BmCount))
    {
        const auto LowerBound = BmEntry.RegionBase;
        const auto UpperBound = BmEntry.RegionBase + BmEntry.RegionSize;

        if (Address >= LowerBound && Address < UpperBound)
        {
            const QWORD PageCount = PAGE_COUNT(Address - BmEntry.RegionBase);
            return BmEntry.BitIndex + PageCount;
        }
    }

    return 0;
}

static PAGE_STATE BmGetPageState(QWORD BitIndex)
{
    const QWORD IndexByte = BitIndex >> 3;
    const QWORD IndexBit  = BitIndex & 0b111;

    if (Bitmap.BmBase[IndexByte] & (1 << IndexBit))
        return PAGE_STATE::Used;

    return PAGE_STATE::Unused;
}

static void BmFreePages(QWORD BitIndex, QWORD PageCount)
{
    for (auto i = 0u; i < PageCount; ++i)
    {
        const QWORD IndexByte = (BitIndex + i) >> 3;
        const QWORD IndexBit = (BitIndex + i) & 0b111;

        Bitmap.BmBase[IndexByte] &= ~(1 << IndexBit);
    }
}

static void BmUsePages(QWORD BitIndex, QWORD PageCount)
{
    for (auto i = 0u; i < PageCount; ++i)
    {
        const QWORD IndexByte = (BitIndex + i) >> 3;
        const QWORD IndexBit = (BitIndex + i) & 0b111;

        Bitmap.BmBase[IndexByte] |= (1 << IndexBit);
    }
}

QWORD BmRequestPages(QWORD RequestedCount)
{
    // To search for free pages, we should traverse the Bitmap.Entries
    // because contiguous bits may not correspond to pages in contiguous regions

    for (const BITMAP_ENTRY& BmEntry : osl::span(Bitmap.Entries, Bitmap.BmCount))
    {
        const QWORD PageCount = REGION_COUNT_PAGE(BmEntry);

        if (PageCount < RequestedCount)
            continue;

        auto UnusedPages = 0u;

        //
        // For each page in the entry
        //
        for(auto BitIndex = BmEntry.BitIndex; BitIndex < BmEntry.BitIndex + PageCount; ++BitIndex)
        {
            if (BmGetPageState(BitIndex) == PAGE_STATE::Used)
            {
                UnusedPages = 0;
                continue;
            }

            //
            // if there are RequestedCount free contiguous pages
            //
            if (++UnusedPages == RequestedCount)
            {
                const QWORD FirstPageIndex = BitIndex - UnusedPages + 1;

                BmUsePages(FirstPageIndex, RequestedCount);
                return BmEntry.RegionBase + (FirstPageIndex - BmEntry.BitIndex) * PAGE_SIZE;
            }
        }
    }

    return 0;
}

void BmReleasePages(QWORD Address, QWORD PageCount)
{
    const QWORD Page = BmBitIndexFromAddress(Address);

    BmFreePages(Page, PageCount);
}

void BmInit()
{
    KMEMORY_MAP_INFO MmInfo {};

    if (!KeGetMemoryMapInfo(MmInfo))
        KePanic("[BM]: KeGetMemoryMapInfo failed");

    const QWORD BitmapCount = MmInfo.UsableEntries;
    const QWORD PageCount   = MmInfo.TotalAvailable / PAGE_SIZE;
    const QWORD TotalSize   = BmSizeFor(PageCount);

    printf("[BM]: Found 0x%lX free pages\n", PageCount);
    printf("[BM]: Bitmap size is 0x%lX bytes (%lu KiB)\n", TotalSize, KiB(TotalSize));

    //
    // We do not want to keep track of the pages used to hold the bitmap, because they won't be usable anyway
    //
    const QWORD PagesUsed = (sizeof(BITMAP_ENTRY) * BitmapCount + TotalSize) / PAGE_SIZE;
    const QWORD BytesUsed = BmSizeFor(PagesUsed);
    const QWORD BmSize = TotalSize - BytesUsed;

    printf("[BM]: Bitmap final size is 0x%lX bytes (%lu KiB)\n", BmSize, KiB(BmSize));

    LPVOID ListAddr = BmEarlyAlloc(sizeof(BITMAP_ENTRY) * BitmapCount);
    LPVOID BmapAddr = BmEarlyAlloc(BmSize);

    if (!ListAddr)
        KePanic("[BM]: Could not allocate bitmap entries, BmEarlyAlloc returned null.");

    if (!BmapAddr)
        KePanic("[BM]: Could not allocate bitmap, BmEarlyAlloc returned null.");

    Bitmap.Entries = reinterpret_cast<BITMAP_ENTRY*>(ListAddr);
    Bitmap.BmCount = BitmapCount;
    Bitmap.BmSize = BmSize;
    Bitmap.BmBase = reinterpret_cast<PBYTE>(BmapAddr);

    printf("[BM]: Allocated bitmap structure\n");
    printf("[BM]: Entries (%p) Bitmap (%p)\n", Bitmap.Entries, Bitmap.BmBase);

    //
    // Set all pages to unused by default
    //
    BmResetBits();

    //
    // Copy usable regions to the bitmap entries and cache their bit indices
    //
    BmLoadMemoryMap();
}













