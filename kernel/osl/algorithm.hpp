#ifndef OS_ALGORITHM_HPP
#define OS_ALGORITHM_HPP


#include <cstddef>
#include <osl/mindef.hpp>


namespace osl
{
    constexpr size_t strlen(const char *str)
    {
        const char *s = str;

        for (; *s; ++s);

        return (s - str);
    }
}

LPVOID ZeroMemory(LPVOID Buffer, QWORD Length);

#endif //OS_ALGORITHM_HPP
