#pragma once

#include "../ipc/ipc/protocol.hpp"
#include "wuphf_command.hpp"
#include <cstdint>
#include <vector>

namespace sabre_pilot::ipc
{
    class Wuphf : public ::ipc::IpcProtocol<WuphfCommand::UniquePtr>
    {
    public:
        using Ptr = Wuphf *;
        using SharedPtr = std::shared_ptr<Wuphf>;
        using UniquePtr = std::unique_ptr<Wuphf>;

    private:
        uint32_t _mcuId = 0;
        std::vector<uint8_t> _buffer;

        std::optional<WuphfCommand::UniquePtr> _parseClientHello();

        std::uint16_t _readU16_be(std::size_t offset) const;
        std::uint32_t _readU32_be(std::size_t offset) const;

    public:
        ::ipc::IpcProtocol<WuphfCommand::UniquePtr>::UniquePtr clone();
        std::optional<WuphfCommand::UniquePtr>
        parseBytes(std::vector<uint8_t> &bytes);
    };
} // namespace sabre_pilot::ipc