#pragma once

#include <cstdint>
#include <memory>
#include <optional>
#include <vector>

namespace ipc
{
    template <typename MessageType>
    class IpcProtocol
    {
    public:
        virtual ~IpcProtocol() = default;
        virtual std::unique_ptr<IpcProtocol<MessageType>> clone() = 0;
        virtual std::optional<MessageType>
        parseBytes(std::vector<uint8_t> &bytes) = 0;
    };
} // namespace ipc