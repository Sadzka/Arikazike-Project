#pragma once
#include <unordered_map>
namespace utils
{

    /// /////////////////////////////////// ///
    /// Adding use of Enum in unordered map ///
    /// /////////////////////////////////// ///
    struct EnumClassHash
    {
        template <typename T>
        std::size_t operator()(T t) const
        {
            return static_cast<std::size_t>(t);
        }
    };

    template <typename Key>
    using HashType = typename std::conditional<std::is_enum<Key>::value, EnumClassHash, std::hash<Key>>::type;

    template <typename Key, typename T>
    using unordered_map = std::unordered_map<Key, T, HashType<Key>>;
}
