#pragma once

#include "../ipc/ipc/protocol.hpp"
#include "wuphf_command.hpp"
#include <cstdint>
#include <vector>

namespace sabre_pilot::ipc
{
    class Wuphf : public ::ipc::IpcProtocol
    {
    public:
        using Ptr = Wuphf *;
        using SharedPtr = std::shared_ptr<Wuphf>;
        using UniquePtr = std::unique_ptr<Wuphf>;

    private:
        uint32_t _mcuId = 0;
        std::vector<uint8_t> _buffer;
        ::ipc::Queue<WuphfCommand::UniquePtr> &_queue;

        std::optional<WuphfCommand::UniquePtr> _parseClientHello();
        std::optional<WuphfCommand::UniquePtr> _parseUartAppend();

        std::uint16_t _readU16_be(std::size_t offset) const;
        std::uint32_t _readU32_be(std::size_t offset) const;

    public:
        Wuphf(::ipc::Queue<WuphfCommand::UniquePtr> &queue);
        ::ipc::IpcProtocol::UniquePtr clone() override;

        void parseBytes(std::vector<uint8_t> &bytes) override;
    };
} // namespace sabre_pilot::ipc