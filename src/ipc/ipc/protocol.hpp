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
        virtual bool _parseBuffer() = 0;

    public:
        virtual ~IpcProtocol() = default;
        virtual void pushBytes(std::span<const uint8_t> bytes) = 0;
    };
} // namespace ipc