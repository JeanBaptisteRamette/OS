#include <std/algorithm.hpp>


LPVOID ZeroMemory(LPVOID Buffer, QWORD Length)
{
    volatile char* Current = (volatile char *)Buffer;

    while (Length)
    {
        *Current++ = 0;
        Length--;
    }

    //
    // Return the pointer to ensure the compiler won't optimize this away
    //
    return Buffer;
}
