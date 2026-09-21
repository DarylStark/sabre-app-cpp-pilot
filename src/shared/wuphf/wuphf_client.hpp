#pragma once

#include "wuphf.hpp"
#include "wuphf_message.hpp"
#include <ipc/client.hpp>
#include <ipc/protocol.hpp>
#include <memory>

namespace sabre::ipc
{
    enum class WuphfClientState
    {
        Pending,
        Done
    };

    class WuphfClient : public Wuphf
    {
    public:
        using Ptr = WuphfClient *;
        using SharedPtr = std::shared_ptr<WuphfClient>;
        using UniquePtr = std::unique_ptr<WuphfClient>;

    private:
        WuphfClientState _state = WuphfClientState::Pending;

        std::optional<WuphfMessage::UniquePtr> _parseServerHello();

    public:
        WuphfClient(::ipc::Queue<std::unique_ptr<IncomingMessage>> &queue,
                    std::size_t bufferSize);
    };

    void sendWuphfMessage(::ipc::IpcClient &client,
                          const WuphfMessage &message);
    void sendWuphfMessage(::ipc::IpcSession &client,
                          const WuphfMessage &message);
} // namespace sabre::ipc