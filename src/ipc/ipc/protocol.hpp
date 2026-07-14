#pragma once

#include <cstdint>
#include <vector>

namespace ipc
{
    template <typename MessageType>
    class IpcProtocol
    {
    public:
        virtual ~IpcProtocol() = default;
        virtual MessageType parseBytes(std::vector<uint8_t> &bytes) = 0;
    };
} // namespace ipc