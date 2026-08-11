#pragma once

#include "queue.hpp"
#include <cstdint>
#include <memory>
#include <optional>
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
        std::uint16_t _readU16_be(std::size_t offset) const;
        std::uint32_t _readU32_be(std::size_t offset) const;

        std::vector<uint8_t> _buffer;

    public:
        IpcProtocol(std::size_t bufferSize);
        virtual ~IpcProtocol() = default;

        void pushBytes(std::span<const uint8_t> bytes);
        void parseBuffer();
    };
} // namespace ipc