#pragma once

#include "protocol.hpp"
#include <ipc/client.hpp>

namespace ipc
{
    template <typename MessageType>
    class TcpIpcClient : public IpcClient<MessageType>
    {
    public:
        TcpIpcClient(std::shared_ptr<IpcProtocol<MessageType>> protocol);
        void setup() override;
        void run() override;
        void stop() override;
    };

    template <typename MessageType>
    TcpIpcClient<MessageType>::TcpIpcClient(
        std::shared_ptr<IpcProtocol<MessageType>> protocol)
        : IpcClient<MessageType>(protocol)
    {
    }

    template <typename MessageType>
    void TcpIpcClient<MessageType>::setup()
    {
    }

    template <typename MessageType>
    void TcpIpcClient<MessageType>::run()
    {
    }

    template <typename MessageType>
    void TcpIpcClient<MessageType>::stop()
    {
    }
} // namespace ipc