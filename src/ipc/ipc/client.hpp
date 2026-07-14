#pragma once

#include "protocol.hpp"
#include <memory>

namespace sabre_pilot::ipc
{
    template <typename MessageType>
    class IpcClient
    {
    protected:
        std::shared_ptr<IpcProtocol<MessageType>> _protocol;

    public:
        IpcClient(std::shared_ptr<IpcProtocol<MessageType>> protocol);
        virtual ~IpcClient() = default;
        virtual void setup() = 0;
        virtual void start() = 0;
        virtual void stop() = 0;
    };
} // namespace sabre_pilot::ipc