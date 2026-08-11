#pragma once

#include "protocol.hpp"
#include "queue.hpp"
#include <memory>

namespace ipc
{
    class IpcServer
    {
    public:
        using Ptr = IpcServer *;
        using SharedPtr = std::shared_ptr<IpcServer>;
        using UniquePtr = std::unique_ptr<IpcServer>;

    protected:
        std::shared_ptr<IpcProtocol> _protocol;

    public:
        IpcServer(std::shared_ptr<IpcProtocol> protocol) : _protocol(protocol)
        {
        }

        virtual ~IpcServer() = default;
        virtual void setup() = 0;
        virtual void run() = 0;
        virtual void stop() = 0;
    };
} // namespace ipc