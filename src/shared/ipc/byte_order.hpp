#pragma once

#include <algorithm>
#include <array>
#include <bit>
#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

namespace ipc::byte_order
{
    template <typename T>
        requires std::is_integral_v<T> && std::is_trivially_copyable_v<T>
    constexpr std::vector<std::byte> serialize(T value)
    {
        std::vector<std::byte> out{sizeof(T)};

        using U = std::make_unsigned_t<T>;
        U u = static_cast<U>(value);

        for (std::size_t i = 0; i < sizeof(T); ++i)
        {
            out[sizeof(T) - 1 - i] = std::byte((u >> (8 * i)) & 0xFF);
        }

        return out;
    }

    template <typename T>
        requires std::is_floating_point_v<T>
    constexpr std::vector<std::byte> serialize(T value)
    {
        std::vector<std::byte> out{sizeof(T)};

        using U = std::conditional_t<
            sizeof(T) == 4, std::uint32_t,
            std::conditional_t<sizeof(T) == 8, std::uint64_t, void>>;

        static_assert(!std::is_same_v<U, void>,
                      "Unsupported floating-point size");

        U bits = std::bit_cast<U>(value);

        for (std::size_t i = 0; i < sizeof(T); ++i)
        {
            out[sizeof(T) - 1 - i] = std::byte((bits >> (8 * i)) & 0xFF);
        }

        return out;
    }

    constexpr std::vector<std::byte> serialize(const std::string &data)
    {
        std::vector<std::byte> out(data.size());
        std::ranges::transform(data, out.begin(),
                               [](unsigned char c) { return std::byte(c); });
        return out;
    } // LCOV_EXCL_LINE

    template <typename T, std::ranges::range R>
        requires std::is_integral_v<T> && std::is_trivially_copyable_v<T>
    constexpr T deserialize(const R &data)
    {
        using U = std::make_unsigned_t<T>;
        U u = 0;

        for (std::size_t i = 0; i < sizeof(T); ++i)
        {
            u |= (static_cast<U>(std::to_integer<unsigned char>(data[i]))
                  << (8 * (sizeof(T) - 1 - i)));
        }

        return static_cast<T>(u);
    }

    template <typename T, std::ranges::range R>
        requires std::is_floating_point_v<T>
    constexpr T deserialize(const R &data)
    {
        using U = std::conditional_t<
            sizeof(T) == 4, std::uint32_t,
            std::conditional_t<sizeof(T) == 8, std::uint64_t, void>>;

        static_assert(!std::is_same_v<U, void>,
                      "Unsupported floating-point size");

        U bits = 0;

        for (std::size_t i = 0; i < sizeof(T); ++i)
        {
            bits |= (static_cast<U>(std::to_integer<unsigned char>(data[i]))
                     << (8 * (sizeof(T) - 1 - i)));
        }

        return std::bit_cast<T>(bits);
    }

    template <std::ranges::range R>
    constexpr std::string deserializeString(const R &data)
    {
        std::string out(data.size(), ' ');
        std::ranges::transform(data, out.begin(), [](std::byte b)
                               { return static_cast<char>(b); });
        return out;
    } // LCOV_EXCL_LINE
} // namespace ipc::byte_order