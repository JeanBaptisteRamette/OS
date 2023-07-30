#ifndef OS_MM_COMMON_HPP
#define OS_MM_COMMON_HPP

#define PAGE_SIZE 4096
#define PAGE_MASK (~(PAGE_SIZE - 1))

//
// Rounds down. "Address" can represent a size
//
#define PAGE_ALIGN(Address) ((Address) & PAGE_MASK)

//
// Rounds up. "Address" can represent a size
//
#define PAGE_ALIGN_UP(Address) (((Address) + PAGE_SIZE - 1) & PAGE_MASK)

#define PAGE_ALIGNED(Address) (((Address) & (PAGE_SIZE - 1)) == 0)

#define PAGE_COUNT(Size) ((Size) / 4096)

#endif //OS_MM_COMMON_HPP
