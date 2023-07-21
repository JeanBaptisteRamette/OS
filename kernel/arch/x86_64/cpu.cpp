#include <arch/x86_64/cpu.hpp>
#include <std/mindef.hpp>


void KeClearInterrupts()
{
    asm volatile ("cli");
}

void KeSetInterrupts()
{
    asm volatile ("sti");
}

bool KeInterruptsEnabled()
{
    QWORD Flags = 0;

    asm volatile (
            "pushf\n"
            "pop %0"
            : "=r" (Flags) : : "memory"       // Clobber list: indicates that inline assembly modifies memory
            );

    return Flags & 0x200;
}

[[noreturn]]
void KeHalt()
{
    //
    // hlt instructions halts until next interrupts so
    // clear interrupts because we want to halt forever
    //
    KeClearInterrupts();

    while (true)
        asm volatile ("hlt");
}