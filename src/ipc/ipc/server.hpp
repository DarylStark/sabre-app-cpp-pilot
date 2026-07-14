#pragma once

#include "protocol.hpp"
#include <memory>

namespace ipc
{
    template <typename MessageType>
    class IpcServer
    {
    protected:
        std::shared_ptr<IpcProtocol<MessageType>> _protocol;

    public:
        IpcServer(std::shared_ptr<IpcProtocol<MessageType>> protocol)
            : _protocol(protocol)
        {
        }

        virtual ~IpcServer() = default;
        virtual void setup() = 0;
        virtual void run() = 0;
        virtual void stop() = 0;
    };
} // namespace ipc