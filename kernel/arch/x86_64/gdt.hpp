#ifndef OS_GDT_HPP
#define OS_GDT_HPP

#include <std/array.hpp>
#include <std/mindef.hpp>

//
// Limine loads a predefined GDT for us, but on the long term for more advanced features such as TSS, UserMode, we will
// want our own, so we might as well write the GDT code now
//

struct SEGMENT_DESCRIPTOR
{
    WORD LoLimit;                // Limit Low Part
    WORD LoBaseAddr;             // Base Address Low Part
    BYTE MiBaseAddr;             // Base Address Middle Part
    BYTE AccessBits;             // Access
    BYTE HiLimitAndFlags;        // Store Limit High Part and
    BYTE HiBaseAddr;             // Base Address High Part
} PACKED;


struct GDT_REGISTER
{
    WORD  Limit;                 // Offset of the last byte in the GDT
    QWORD BaseAddress;           // Pointer to the first byte of the GDT
} PACKED;


constexpr WORD SEG_NULL = 0;    // Mandatory null descriptor is the first entry
constexpr WORD SEG_K64C = 1;
constexpr WORD SEG_K64D = 2;

constexpr WORD SELECTOR_KCS = SEG_K64C * 8;    // 0x8
constexpr WORD SELECTOR_KDS = SEG_K64D * 8;    // 0x10

void KeGdtLoad();


#endif //OS_GDT_HPP
