#pragma once

#include "protocol.hpp"
#include "queue.hpp"
#include <memory>

namespace ipc
{
    template <typename MessageType>
    class IpcServer
    {
    protected:
        std::shared_ptr<IpcProtocol<MessageType>> _protocol;
        Queue<MessageType> &_queue;

    public:
        IpcServer(std::shared_ptr<IpcProtocol<MessageType>> protocol,
                  Queue<MessageType> &queue)
            : _protocol(protocol), _queue(queue)
        {
        }

        virtual ~IpcServer() = default;
        virtual void setup() = 0;
        virtual void run() = 0;
        virtual void stop() = 0;
    };
} // namespace ipc