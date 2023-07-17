#include <arch/x86_64/cpu.hpp>
#include <limine.h>
#include <serial.hpp>
#include <std/printf.hpp>


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

    printf("XXXXX 0x%X XXXXX\n", 0xDEAD);
    printf("XXXXX 0x%X XXXXX\n", 0xBEEF);

    KeHalt();
}

