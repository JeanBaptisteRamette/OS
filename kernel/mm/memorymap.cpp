#include <std/string_view.hpp>
#include <std/printf.hpp>
#include <mm/memorymap.hpp>
#include <std/span.hpp>


static volatile limine_memmap_request MemoryMapRequest = {
        .id = LIMINE_MEMMAP_REQUEST,
        .revision = 0,
        .response = nullptr
};


limine_memmap_response* KeGetMemoryMap()
{
    //
    // The memory map may be modified carefully by the caller
    //
    return MemoryMapRequest.response;
}

void DbgDisplayMemoryMap()
{
    const auto MemoryMap = KeGetMemoryMap();

    if (!MemoryMap)
        return;

    auto EntryTypeDesc = [](QWORD Type) -> osl::string_view
    {
        switch (Type)
        {
            case LIMINE_MEMMAP_USABLE:
                return "Usable RAM";
            case LIMINE_MEMMAP_RESERVED:
                return "Reserved";
            case LIMINE_MEMMAP_ACPI_RECLAIMABLE:
                return "ACPI reclaimable";
            case LIMINE_MEMMAP_ACPI_NVS:
                return "ACPI NVS";
            case LIMINE_MEMMAP_BAD_MEMORY:
                return "Bad memory";
            case LIMINE_MEMMAP_FRAMEBUFFER:
                return "Framebuffer";
            case LIMINE_MEMMAP_BOOTLOADER_RECLAIMABLE:
                return "Bootloader reclaimable";
            case LIMINE_MEMMAP_KERNEL_AND_MODULES:
                return "Kernel/Modules";
            default:
                return "???";
        }
    };

    QWORD TotalMemory = 0;
    QWORD TotalUsable = 0;

    const osl::span<limine_memmap_entry*> MapView(MemoryMap->entries, MemoryMap->entry_count);

    for (const auto& Entry : MapView)
    {
        if (!Entry)
            continue;

        const PCSTR RegionDesc = EntryTypeDesc(Entry->type).data();
        const QWORD RegionSize = Entry->length;
        const QWORD RegionBase = Entry->base;
        const QWORD RegionEnd  = RegionBase + RegionSize;

        printf("[%012lX - %012lX] %012lX %s\n", RegionBase, RegionEnd, RegionSize, RegionDesc);

        TotalMemory += RegionSize;

        if (Entry->type == LIMINE_MEMMAP_USABLE)
            TotalUsable += RegionSize;
    }

    printf("Total Memory: %lu MiB\n", MiB(TotalMemory));
    printf("Total Usable: %lu MiB\n", MiB(TotalUsable));
}

QWORD KeMemMapGetUsableMemory()
{
    QWORD TotalAvailable = 0;

    const auto MemoryMap = KeGetMemoryMap();

    if (!MemoryMap)
        return 0;

    const osl::span<limine_memmap_entry*> MapView(MemoryMap->entries, MemoryMap->entry_count);

    for (const auto& Entry : MapView)
        if (Entry && Entry->type == LIMINE_MEMMAP_USABLE)
            TotalAvailable += Entry->length;

    return TotalAvailable;
}
