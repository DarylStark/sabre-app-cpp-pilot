#pragma once

#include "wuphf_message.hpp"
#include <cstdint>
#include <functional>
#include <ipc/client.hpp>
#include <ipc/protocol.hpp>
#include <optional>
#include <unordered_map>
#include <vector>

namespace sabre::ipc
{
    using ParseMethod = std::function<std::optional<WuphfMessage::UniquePtr>()>;

    class Wuphf : public ::ipc::IpcProtocol
    {
    protected:
        ::ipc::Queue<WuphfMessage::UniquePtr> &_queue;
        std::unordered_map<uint32_t, ParseMethod> _parseMethods;

        std::size_t _parseOnePacket() override;

    public:
        using Ptr = Wuphf *;
        using SharedPtr = std::shared_ptr<Wuphf>;
        using UniquePtr = std::unique_ptr<Wuphf>;

    public:
        Wuphf(::ipc::Queue<WuphfMessage::UniquePtr> &queue,
              std::size_t bufferSize);
    };

    void sendWuphfMessage(::ipc::IpcClient &client,
                          const WuphfMessage &message);
    void sendWuphfMessage(::ipc::IpcSession &client,
                          const WuphfMessage &message);
} // namespace sabre::ipc