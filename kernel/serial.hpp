#ifndef OS_SERIAL_HPP
#define OS_SERIAL_HPP


#include <arch/x86_64/io.hpp>

//
// Documentation:
// https://www.lammertbies.nl/comm/info/serial-uart
// https://onlinedocs.microchip.com/oxy/GUID-199548F4-607C-436B-80C7-E4F280C1CAD2-en-US-1/GUID-CBB4A20C-7A5B-4DEF-8F0C-4CC95186FF29.html
//

namespace
{
    //
    // Communication port's address in the I/O ports address space
    //
    constexpr WORD COM1 = 0x3F8;

    //
    // READ - Receiver Buffer Register
    //
    constexpr WORD RBR = COM1 + 0;

    //
    // WRITE - Transmitter Holding Register
    //
    constexpr WORD THR = COM1 + 0;

    //
    // Divisor Latch Low/High when DLAB is set
    //
    constexpr WORD DLL  = COM1 + 0;
    constexpr WORD DLH  = COM1 + 1;

    //
    // READ/WRITE - Interrupt Enable Register
    //
    constexpr WORD IER = COM1 + 1;

    //
    // WRITE - FIFO Control Register
    //
    constexpr WORD FCR = COM1 + 2;

    //
    // READ/WRITE - Line Control Register
    //
    constexpr WORD LCR = COM1 + 3;

    //
    // READ/WRITE - Modem Control Register
    //
    constexpr WORD MCR = COM1 + 4;

    //
    // READ - Line Status Register
    //
    constexpr WORD LSR = COM1 + 5;


}

bool SerialComInit();
BYTE SerialComRead();
void SerialComWrite(BYTE Data);

#endif //OS_SERIAL_HPP
