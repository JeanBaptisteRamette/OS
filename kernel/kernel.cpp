#include <serial.hpp>
#include <std/printf.hpp>
#include <arch/x86_64/cpu.hpp>
#include <arch/x86_64/gdt.hpp>
#include <arch/x86_64/idt.hpp>


extern "C"
[[noreturn]] void KernelEntry()
{
    //
    // Kernel Entry Point
    //


    //
    // Init serial communication for logging to host machine
    //
    if (!SerialComInit())
        KeHaltForever();

    // We should make sure to clear interrupts before loading our GDT and IDT
    // Limine should actually clear IF for us, but I prefer to keep this here
    if (KeInterruptsEnabled())
        KeClearInterrupts();

    KeGdtLoad();
    KeIdtLoad();

    printf("Loaded GDT !\n");
    printf("Loaded IDT !\n");

    printf("Triggering processor exception 6\n");

    asm __volatile__ ("int $6");

    printf("Interrupt 6 handled\n");

    KeHaltForever();
}

