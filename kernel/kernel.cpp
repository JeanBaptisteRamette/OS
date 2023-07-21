#include <limine.h>
#include <serial.hpp>
#include <std/printf.hpp>
#include <arch/x86_64/cpu.hpp>
#include <arch/x86_64/gdt.hpp>


extern "C"
void KernelEntry()
{
    //
    // Kernel Entry Point
    //


    //
    // Init serial communication for logging to host machine
    //
    if (!SerialComInit())
        KeHalt();

    KeGdtLoad();

    printf("Loaded Global Descriptor Table !\n");

    KeHalt();
}

