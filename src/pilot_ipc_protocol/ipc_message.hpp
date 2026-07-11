#pragma once

#include <cstdint>
#include <vector>

namespace sabre_pilot::ipc
{
    class IpcMessage
    {
    public:
        virtual void parseBytes(std::vector<uint8_t> data) = 0;
    };
} // namespace sabre_pilot::ipc