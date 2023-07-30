#include <arch/x86_64/pic.hpp>
#include <arch/x86_64/io.hpp>
#include <arch/x86_64/isr.hpp>
#include <std/mindef.hpp>


//
// IO-space base address for master and slave PIC
//
constexpr WORD MPIC = 0x20;
constexpr WORD SPIC = 0xA0;

//
// IO ports for the master PIC, A is command, B is data
//
constexpr WORD MPICA = MPIC + 0;
constexpr WORD MPICB = MPIC + 1;

//
// IO ports for the slave PIC, A is command, B is data
//
constexpr WORD SPICA = SPIC + 0;
constexpr WORD SPICB = SPIC + 1;

constexpr BYTE ICW1_INIT = 0x10;
constexpr BYTE ICW1_ICW4 = 0x01;


void KePicDisable()
{
    //
    // Operation Control Word 1 (OCW1):
    //      Sets and clears the mask bits in the interupt mask register
    //      M = 1: channel is masked
    //      M = 0: channel is enabled

    // Mask all channel in the master and slave PIC
    HalIoSendByte(MPICB, 0b1111'1111);
    HalIoSendByte(SPICB, 0b1111'1111);
}

void KePicRemapIRQs()
{
    //
    // Send Initialization Command Word 1 (ICW1) to the PICs
    // to tell them we want to reconfigure them.
    // After that the PICs will expect 3 ICWs.
    //
    HalIoSendByte(MPICA, ICW1_INIT | ICW1_ICW4);
    HalIoSendByte(SPICA, ICW1_INIT | ICW1_ICW4);

    //
    // ICW2: Configure base vector numbers for IRQs. This is the important bit
    //       - 32 is right after the CPU exceptions
    //       - 32 + 8 is for the slave PIC, as each PIC handle 8 IRQs
    //
    HalIoSendByte(MPICB, CPU_EXCEPTION_COUNT);
    HalIoSendByte(SPICB, CPU_EXCEPTION_COUNT + 8);

    //
    // ICW3: Specifies what Interrupt Request (IRQ) is connected to slave PIC
    //       IRQ number the master PIC uses to connect to (In binary notation)
    HalIoSendByte(MPICB, 0b0000'0100);
    HalIoSendByte(SPICB, 0b0000'0010);

    //
    // ICW4: Additional operations modes, no additional, clear all
    //
    HalIoSendByte(MPICB, 0b0000'0001);
    HalIoSendByte(SPICB, 0b0000'0001);
}