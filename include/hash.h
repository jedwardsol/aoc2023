#pragma once

#include <cstddef>
#include <functional>
#include <array>
#include <ranges>


template<typename T>
concept Hashable = requires(T a)
{
    { std::hash<T>{}(a) } -> std::convertible_to<std::size_t>;
};


// from boost
template <Hashable T>
inline void hash_combine(std::size_t &hash, const T &v) 
{
    std::hash<T> hasher;
    hash ^= hasher(v) + 0x9e3779b9 + (hash<<6) + (hash>>2);
}

inline void hash_combine(std::size_t &hash, std::ranges::random_access_range auto &&range)
{
    for(auto const &e : range)
    {
        hash_combine(hash,e);
    }
}
