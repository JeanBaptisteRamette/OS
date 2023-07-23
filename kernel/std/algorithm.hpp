#ifndef OS_ALGORITHM_HPP
#define OS_ALGORITHM_HPP


#include <cstddef>


namespace osl
{
    constexpr size_t strlen(const char *str)
    {
        const char *s = str;

        for (; *s; ++s);

        return (s - str);
    }
}


#endif //OS_ALGORITHM_HPP
