#ifndef OS_IO_HPP
#define OS_IO_HPP

#include <osl/mindef.hpp>


inline BYTE HalIoRecvByte(WORD port)
{
    BYTE value;
    __asm__ volatile ("inb %1, %0" : "=a"(value) : "d"(port));
    return value;
}

inline WORD HalIoRecvWord(WORD port)
{
    WORD value;
    __asm__ volatile ("inw %1, %0" : "=a"(value) : "d"(port));
    return value;
}

inline DWORD HalIoRecvDword(WORD port)
{
    DWORD value;
    __asm__ volatile ("inl %1, %0" : "=a"(value) : "d"(port));
    return value;
}

inline void HalIoSendByte(WORD port, BYTE value)
{
    __asm__ volatile ("outb %0, %1" ::"a"(value), "d"(port));
}

inline void HalIoSendWord(WORD port, WORD value)
{
    __asm__ volatile ("outw %0, %1" ::"a"(value), "d"(port));
}

inline void HalIoSendDword(WORD port, DWORD value)
{
    __asm__ volatile ("outl %0, %1" ::"a"(value), "d"(port));
}


#endif //OS_IO_HPP
