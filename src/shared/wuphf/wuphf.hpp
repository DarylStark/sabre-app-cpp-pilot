#pragma once

#include "wuphf_message.hpp"
#include <cstdint>
#include <functional>
#include <ipc/client.hpp>
#include <ipc/protocol.hpp>
#include <unordered_map>
#include <vector>

namespace sabre::ipc
{
    using ParseMethod = std::function<std::optional<WuphfMessage::UniquePtr>()>;

    struct IncomingMessage
    {
        std::shared_ptr<::ipc::IpcSession> session;
        WuphfMessage::UniquePtr message;
    };

    class Wuphf : public ::ipc::IpcProtocol
    {
    protected:
        ::ipc::Queue<std::unique_ptr<IncomingMessage>> &_queue;
        std::unordered_map<uint32_t, ParseMethod> _parseMethods;

        std::size_t _parseOnePacket() override;

    public:
        using Ptr = Wuphf *;
        using SharedPtr = std::shared_ptr<Wuphf>;
        using UniquePtr = std::unique_ptr<Wuphf>;

    public:
        Wuphf(::ipc::Queue<std::unique_ptr<IncomingMessage>> &queue,
              std::size_t bufferSize);
    };

    enum class WuphfServerState
    {
        Pending,
        Done
    };

    class WuphfServer : public Wuphf
    {
    public:
        using Ptr = WuphfServer *;
        using SharedPtr = std::shared_ptr<WuphfServer>;
        using UniquePtr = std::unique_ptr<WuphfServer>;

    private:
        uint32_t _mcuId = 0;

        std::optional<WuphfMessage::UniquePtr> _parseClientHello();
        std::optional<WuphfMessage::UniquePtr> _parseUartAppend();

        WuphfServerState _state = WuphfServerState::Pending;

    public:
        WuphfServer(::ipc::Queue<std::unique_ptr<IncomingMessage>> &queue,
                    std::size_t bufferSize);
    };

    class WuphfClient : public Wuphf
    {
    public:
        using Ptr = WuphfClient *;
        using SharedPtr = std::shared_ptr<WuphfClient>;
        using UniquePtr = std::unique_ptr<WuphfClient>;

    public:
        WuphfClient(::ipc::Queue<std::unique_ptr<IncomingMessage>> &queue,
                    std::size_t bufferSize);
    };

    void sendWuphfMessage(::ipc::IpcClient &client,
                          const WuphfMessage &message);
    void sendWuphfMessage(::ipc::IpcSession &client,
                          const WuphfMessage &message);
} // namespace sabre::ipc