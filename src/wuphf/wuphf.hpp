#pragma once

#include "../ipc/ipc/protocol.hpp"
#include "wuphf_message.hpp"
#include <cstdint>
#include <vector>

namespace sabre_pilot::ipc
{
    class Wuphf : public ::ipc::IpcProtocol<WuphfMessage>
    {
    public:
        WuphfMessage parseBytes(std::vector<uint8_t> &bytes);
    };
} // namespace sabre_pilot::ipc