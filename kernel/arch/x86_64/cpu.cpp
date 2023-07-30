#include <arch/x86_64/cpu.hpp>
#include <std/mindef.hpp>


void KeClearInterrupts()
{
    //
    // Does not affect non-maskable interrupts (NMIs)
    //
    asm volatile ("cli");
}

void KeSetInterrupts()
{
    asm volatile ("sti");
}

bool KeInterruptsEnabled()
{
    QWORD Flags = 0;

    //
    // pop RFLAGS into variable
    //
    asm volatile (
            "pushf\n"
            "pop %0"
            : "=r" (Flags) : : "memory"       // Clobber list: indicates that inline assembly modifies memory
            );

    //
    // Checks if IF (Interrupt Flag - bit 9) of RFLAGS is set
    //
    return Flags & 0x200;
}

void KeHalt()
{
    asm volatile ("hlt");
}

[[noreturn]]
void KeHaltForever()
{
    while (true)
        KeHalt();
}