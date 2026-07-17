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
        std::vector<uint8_t> _buffer;

        std::optional<WuphfMessage> _parseClientHello();

        std::uint16_t _readU16_be(std::size_t offset) const;
        std::uint32_t _readU32_be(std::size_t offset) const;

    public:
        std::unique_ptr<::ipc::IpcProtocol<WuphfMessage>> clone();
        std::optional<WuphfMessage> parseBytes(std::vector<uint8_t> &bytes);
    };
} // namespace sabre_pilot::ipc