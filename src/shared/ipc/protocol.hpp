#pragma once

#include "queue.hpp"
#include "types.hpp"
#include <cstdint>
#include <ipc/byte_order.hpp>
#include <memory>
#include <optional>
#include <ranges>
#include <span>
#include <vector>

namespace ipc
{
    class IpcProtocol
    {
    public:
        using Ptr = IpcProtocol *;
        using SharedPtr = std::shared_ptr<IpcProtocol>;
        using UniquePtr = std::unique_ptr<IpcProtocol>;
        virtual std::size_t _parseOnePacket() = 0;

    protected:
        BufferType _buffer;

        template <typename T>
        constexpr T _deserialize(std::size_t startIndex)
        {
            return ::ipc::byte_order::deserialize<T>(
                _buffer | std::views::drop(startIndex) |
                std::views::take(sizeof(T)));
        }

    public:
        IpcProtocol(std::size_t bufferSize);
        virtual ~IpcProtocol() = default;

        void pushBytes(std::span<const std::byte> bytes);
        void parseBuffer();
    };
} // namespace ipc