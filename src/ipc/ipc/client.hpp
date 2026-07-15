#pragma once

#include "protocol.hpp"
#include <memory>

namespace ipc
{
    template <typename MessageType>
    class IpcClient
    {
    protected:
        std::shared_ptr<IpcProtocol<MessageType>> _protocol;

    public:
        IpcClient(std::shared_ptr<IpcProtocol<MessageType>> protocol)
            : _protocol(protocol)
        {
        }

        virtual ~IpcClient() = default;
        virtual void setup() = 0;
        virtual void run() = 0;
        virtual void stop() = 0;

        // TODO: Do we need these?
        virtual bool waitForConnection() = 0;
        virtual void sendData(const std::string &data) = 0;
    };
} // namespace ipc