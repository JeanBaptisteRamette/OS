#ifndef OS_OSL_ARRAY_HPP
#define OS_OSL_ARRAY_HPP


#include <cstddef>


namespace osl
{
    template<typename T, size_t Size>
    class array
    {
        static_assert(Size != 0, "array parameter Size must be non-zero");

        using value_type      = T;
        using size_type       = size_t;
        using reference       = value_type&;
        using const_reference = const value_type&;
        using pointer         = value_type*;
        using const_pointer   = const value_type*;
        using iterator        = pointer;
        using const_iterator  = const_pointer;

    public:
        array(const array&) = default;
        array(array&&) noexcept = default;

        array& operator=(const array&) = default;
        array& operator=(array&&) noexcept = default;

        [[nodiscard]] constexpr       iterator  begin()       noexcept { return elements; }
        [[nodiscard]] constexpr const_iterator  begin() const noexcept { return elements; }
        [[nodiscard]] constexpr const_iterator cbegin() const noexcept { return elements; }

        [[nodiscard]] constexpr       iterator  end()       noexcept { return elements + Size; }
        [[nodiscard]] constexpr const_iterator  end() const noexcept { return elements + Size; }
        [[nodiscard]] constexpr const_iterator cend() const noexcept { return elements + Size; }

        [[nodiscard]] constexpr size_type size()     const noexcept { return Size; }
        [[nodiscard]] constexpr size_type max_size() const noexcept { return Size; }  // for template
        [[nodiscard]] constexpr bool empty()         const noexcept { return Size == 0; }     // for template

        [[nodiscard]] constexpr       reference operator[](size_type pos)       { return elements[pos]; }
        [[nodiscard]] constexpr const_reference operator[](size_type pos) const { return elements[pos]; }

        [[nodiscard]] constexpr       reference at(size_type pos)       { return elements[pos]; }
        [[nodiscard]] constexpr const_reference at(size_type pos) const { return elements[pos]; }

        [[nodiscard]] constexpr       reference front()       { return elements[0]; }
        [[nodiscard]] constexpr const_reference front() const { return elements[0]; }

        [[nodiscard]] constexpr       reference back()       { return elements[Size - 1]; }
        [[nodiscard]] constexpr const_reference back() const { return elements[Size - 1]; }

        [[nodiscard]] constexpr       pointer data()       noexcept { return elements; }
        [[nodiscard]] constexpr const_pointer data() const noexcept { return elements; }

        [[nodiscard]] constexpr bool operator==(const array& other) const
        {
            for (decltype(Size) i = 0; i < Size; ++i)
                if (elements[i] != other[i])
                    return false;

            return true;
        }

        value_type elements[Size];
    };
}

#endif //OS_OSL_ARRAY_HPP
