#pragma once

#include "../ipc/ipc/protocol.hpp"
#include "wuphf_message.hpp"
#include <cstdint>
#include <vector>

namespace sabre_pilot::ipc
{
    class Wuphf : public ::ipc::IpcProtocol<WuphfMessage>
    {
    private:
        uint32_t _mcuId = 0;

    public:
        std::unique_ptr<::ipc::IpcProtocol<WuphfMessage>> clone();
        WuphfMessage parseBytes(std::vector<uint8_t> &bytes);
    };
} // namespace sabre_pilot::ipc