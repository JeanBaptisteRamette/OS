#ifndef OS_STRING_VIEW_HPP
#define OS_STRING_VIEW_HPP


#include <cstddef>
#include <osl/algorithm.hpp>


namespace osl
{
    class string_view
    {
        using value_type = char;
        using size_type = size_t;

        // only const because it's a view
        using const_pointer = const value_type*;
        using const_iterator = const_pointer;

    public:
        constexpr string_view() : data_(nullptr), size_(0) {}
        constexpr string_view(const_pointer str) :  data_(str), size_(osl::strlen(str)) {}
        constexpr string_view(const_pointer str, size_type length) : data_(str), size_(length) {}

        constexpr string_view(const string_view& other) noexcept = default;
        constexpr string_view(string_view&& other) noexcept = default;

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

#endif //OS_STRING_VIEW_HPP
