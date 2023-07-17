#ifndef OS_CPU_HPP
#define OS_CPU_HPP


static void KeClearInterrupt()
{
    asm volatile ("cli");
}

[[noreturn]]
static void KeHalt()
{
    //
    // hlt instructions halts until next interrupts so
    // clear interrupts because we want to halt forever
    //
    KeClearInterrupt();

    while (true)
        asm volatile ("hlt");
}

#endif //OS_CPU_HPP
