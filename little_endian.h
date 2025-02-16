#pragma once

#include <climits>
#include <cstddef>
#include <cstdint>
#include <array>
#include <concepts>

template <std::unsigned_integral T>
T ReadLittleEndian(const char* little_endian_bytes) {
    T result = 0;
    for (size_t i = 0; i < sizeof(T); ++i) {
        result += static_cast<T>(static_cast<const uint8_t>(little_endian_bytes[i])) << i * CHAR_BIT;
    }
    return result;
}

template <std::signed_integral T>
T ReadLittleEndian(const char* little_endian_bytes) {
    using NewType = std::make_unsigned_t<T>;
    NewType temp = ReadLittleEndian<NewType>(little_endian_bytes);
    return *reinterpret_cast<T*>(&temp);
}

template <std::unsigned_integral T>
std::array<char, sizeof(T)> TransformToLittleEndian(T bytes) {
    std::array<char, sizeof(T)> result{};
    for (size_t i = 0; i < sizeof(T); ++i) {
        uint8_t temp = bytes >> CHAR_BIT * i;
        result[i] = reinterpret_cast<char&>(temp);
    }
    return result;
}

template <std::signed_integral T>
std::array<char, sizeof(T)> TransformToLittleEndian(T bytes) {
    using NewType = std::make_unsigned_t<T>;
    return TransformToLittleEndian<NewType>(*reinterpret_cast<T*>(&bytes));
}