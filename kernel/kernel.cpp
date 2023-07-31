#include <serial.hpp>
#include <osl/printf.hpp>
#include <arch/x86_64/cpu.hpp>
#include <arch/x86_64/gdt.hpp>
#include <arch/x86_64/idt.hpp>
#include <arch/x86_64/pic.hpp>
#include <mm/memorymap.hpp>
#include <mm/pmm.hpp>


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

    //
    // The PIC is a legacy and obsolete controller which IRQs overlap CPU exceptions
    //
    KePicRemapIRQs();
    KePicDisable();

    DbgDisplayMemoryMap();

    KePmmInit();

    const QWORD Page1 = KePmmRequest(1);
    const QWORD Page2 = KePmmRequest(4097);
    const QWORD Page3 = KePmmRequest(8192);

    printf("Allocated physical memory: %lX\n", Page1);
    printf("Allocated physical memory: %lX\n", Page2);
    printf("Allocated physical memory: %lX\n", Page3);

    KePmmRelease(Page1, 4097);
    KePmmRelease(Page2, 1);
    KePmmRelease(Page3, 8192);


    KeHaltForever();
}

