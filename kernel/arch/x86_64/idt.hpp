#ifndef OS_IDT_HPP
#define OS_IDT_HPP


#include <std/mindef.hpp>


struct GATE_DESCRIPTOR
{
    WORD  LoIsrOffset;            // Low part of the Interrupt Service Routine offset inside the code segment pointed by SegmentSelector
    WORD  SegmentSelector;        // Segment selector to the code segment in which the ISR is
    BYTE  InterruptStackTable;
    BYTE  Attributes;
    WORD  MiIsrOffset;            // Middle part of the Interrupt Service Routine
    DWORD HiIsrOffset;            // High part of the Interrupt Service Routine
    DWORD Reserved;
} PACKED;


struct IDT_REGISTER
{
    WORD  Limit;                 // Offset of the last byte in the IDT
    QWORD BaseAddress;           // Pointer to the first byte of the IDT
} PACKED;


void KeIdtLoad();


#endif //OS_IDT_HPP
