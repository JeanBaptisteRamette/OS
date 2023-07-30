#ifndef OS_SPAN_HPP
#define OS_SPAN_HPP

#include <cstddef>

//
// a span is an array view, much like a string view
//

namespace osl
{
    template<typename T>
    class span
    {
    public:
        using value_type = T;
        using size_type = size_t;

        // only const because it's a view
        using const_pointer = const value_type*;
        using const_iterator = const_pointer;

    public:
        constexpr span() : data_(nullptr), size_(0) {}
        constexpr span(const_pointer ptr, size_type length) : data_(ptr), size_(length) {}

        constexpr span(const span& other) noexcept = default;
        constexpr span(span&& other) noexcept = default;

        [[nodiscard]] constexpr const_iterator  begin() const noexcept { return data_; }
        [[nodiscard]] constexpr const_iterator cbegin() const noexcept { return data_; }

        [[nodiscard]] constexpr const_iterator  end() const noexcept { return data_ + size_; }
        [[nodiscard]] constexpr const_iterator cend() const noexcept { return data_ + size_; }

        [[nodiscard]] constexpr size_type size() const noexcept { return size_; }
        [[nodiscard]] constexpr const_pointer data() const noexcept { return data_; }
        [[nodiscard]] constexpr bool empty() const noexcept { return size_ == 0; }

        [[nodiscard]] constexpr value_type operator[](size_type pos) { return data_[pos]; }

    private:
        const_pointer data_;
        size_type size_;
    };
}

#endif //OS_SPAN_HPP
