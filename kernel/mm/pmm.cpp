#include <allocators/localbm.hpp>
#include <mm/common.hpp>
#include <mm/pmm.hpp>


void KePmmInit()
{
    BmInit();
}

QWORD KePmmRequest(QWORD Size)
{
    if (!PAGE_ALIGNED(Size))
        Size = PAGE_ALIGN_UP(Size);

    const QWORD PageCount = PAGE_COUNT(Size);

    return BmRequestPages(PageCount);
}

void KePmmRelease(QWORD Address, QWORD AllocationSize)
{
    if (!PAGE_ALIGNED(Address))
        Address = PAGE_ALIGN(Address);

    if (!PAGE_ALIGNED(AllocationSize))
        AllocationSize = PAGE_ALIGN_UP(AllocationSize);

    const QWORD PageCount = PAGE_COUNT(AllocationSize);

    BmReleasePages(Address, PageCount);
}