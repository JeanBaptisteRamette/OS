#include <serial.hpp>


bool SerialComInit()
{
    //
    // Disable interrupts
    //
    HalIoSendByte(IER, 0x00);

    //
    // Enable DLAB to set baud rate divisor value by setting LCR MSB
    //
    HalIoSendByte(LCR, 0x80);

    //
    // Set divisor to 0x0003 -> 38400 baud
    //
    HalIoSendByte(DLL, 0x03);
    HalIoSendByte(DLH, 0x00);

    //
    // Use 8 bits character length (and disable DLAB)
    //
    HalIoSendByte(LCR, 0b11);

    //
    // Enable FIFO's (receive and transmit queues) and clear them
    // Set receive interrupt trigger level to 14-bytes
    //
    HalIoSendByte(FCR, 0xC7);

    //
    // OUT2, RTS, DTR set
    //
    HalIoSendByte(MCR, 0x0B);

    //
    // Set to loopback mode and send dummy byte to test the serial chip
    //
    HalIoSendByte(MCR, 0x1E);
    HalIoSendByte(THR, 0xAE);

    //
    // Checks the UART echoes it properly
    //
    if (HalIoRecvByte(RBR) != 0xAE)
        return false;

    //
    // Remove loopback mode, OUT1, OUT2, RTS, DTR set
    //
    HalIoSendByte(MCR, 0x0F);

    return true;
}

static bool SerialComDataReady()
{
    //
    // Checks if DR flag (Data Ready) of the LSR is set indicating a data byte is received and stored in the receive buffer or the FIFO.
    // DR is cleared to 0 when the processor reads the data from the receive buffer or the FIFO.
    //

    return HalIoRecvByte(LSR) & 1;
}

BYTE SerialComRead()
{
    while (true)
    {
        if (SerialComDataReady())
        {
            return HalIoRecvByte(RBR);
        }
    }
}

static bool SerialComTxQueueEmpty()
{
    //
    // Checks wether the THR is empty, meaning that the UART is ready
    // to transmit a new data byte. 5th bit of the LSR (counting 0 bit)
    // is set when the Tx FIFO is empty.
    //

    return HalIoRecvByte(LSR) & (1 << 5);
}

void SerialComWrite(BYTE Data)
{
    while (true)
    {
        if (SerialComTxQueueEmpty())
        {
            HalIoSendByte(THR, Data);
            break;
        }
    }
}