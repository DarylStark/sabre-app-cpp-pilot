#pragma once

#include "wuphf_command.hpp"
#include <cstdint>
#include <functional>
#include <ipc/protocol.hpp>
#include <unordered_map>
#include <vector>

namespace sabre_pilot::ipc
{
    using ParseMethod = std::function<std::optional<WuphfCommand::UniquePtr>()>;

    class Wuphf : public ::ipc::IpcProtocol
    {
    public:
        using Ptr = Wuphf *;
        using SharedPtr = std::shared_ptr<Wuphf>;
        using UniquePtr = std::unique_ptr<Wuphf>;

    private:
        uint32_t _mcuId = 0;
        ::ipc::Queue<WuphfCommand::UniquePtr> &_queue;

        std::size_t _parseOnePacket() override;

        std::optional<WuphfCommand::UniquePtr> _parseClientHello();
        std::optional<WuphfCommand::UniquePtr> _parseUartAppend();

        std::unordered_map<uint32_t, ParseMethod> _parseMethods;

    public:
        Wuphf(::ipc::Queue<WuphfCommand::UniquePtr> &queue,
              std::size_t bufferSize);
    };
} // namespace sabre_pilot::ipc