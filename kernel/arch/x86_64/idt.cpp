#include <arch/x86_64/idt.hpp>
#include <arch/x86_64/gdt.hpp>
#include <arch/x86_64/isr.hpp>


#define PRESENT        (1 << 7)
#define DPL0           (0)
#define INTERRUPT_GATE (0b1110)


static osl::array<GATE_DESCRIPTOR, 255> Descriptors {};


GATE_DESCRIPTOR IdtEncodeDescriptor(QWORD IsrOffset, BYTE Attributes)
{
    GATE_DESCRIPTOR desc {};

    desc.LoIsrOffset = LOWORD(IsrOffset);
    desc.MiIsrOffset = HIWORD(LODWORD(IsrOffset));
    desc.HiIsrOffset = HIDWORD(IsrOffset);

    desc.SegmentSelector = SELECTOR_KCS;
    desc.Attributes = Attributes;
    desc.InterruptStackTable = 0;
    desc.Reserved = 0;

    return desc;
}

void KeIdtSetup()
{
    //
    // Assign the processor exceptions handlers
    //
    for (WORD i = 0; i < CPU_EXCEPTION_COUNT; ++i)
        Descriptors[i] = IdtEncodeDescriptor(InterruptsHandlers[i], PRESENT | DPL0 | INTERRUPT_GATE);
}


void KeIdtLoad()
{
    KeIdtSetup();

    const auto Limit   = Descriptors.size() * sizeof(GATE_DESCRIPTOR) - 1;
    const auto Address = (QWORD)Descriptors.data();

    const GDT_REGISTER IdtRegister = {
            Limit,
            Address
    };

    asm __volatile__ ("lidt %0" : : "m"(IdtRegister));
}
