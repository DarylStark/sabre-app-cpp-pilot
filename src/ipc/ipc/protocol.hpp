#pragma once

#include <cstdint>
#include <vector>

namespace sabre_pilot::ipc
{
    template <typename MessageType>
    class IpcProtocol
    {
    public:
        virtual MessageType parseBytes(std::vector<uint8_t> &bytes) = 0;
    };
} // namespace sabre_pilot::ipc