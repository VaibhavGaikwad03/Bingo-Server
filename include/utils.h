#ifndef UTILS_H
#define UTILS_H

#include <type_traits>

namespace utils
{
    template <typename E>
    constexpr auto to_underlying(E e) noexcept
    {
        static_assert(std::is_enum_v<E>, "to_underlying() requires enum type");
        return static_cast<std::underlying_type_t<E>>(e);
    }
}

#endif // UTILS_H