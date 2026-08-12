#pragma once

#include "protocol.hpp"
#include "queue.hpp"
#include <functional>
#include <memory>

namespace ipc
{
    using ProtocolFactory = std::function<std::unique_ptr<IpcProtocol>()>;

    class IpcServer
    {
    public:
        using Ptr = IpcServer *;
        using SharedPtr = std::shared_ptr<IpcServer>;
        using UniquePtr = std::unique_ptr<IpcServer>;

    protected:
        ProtocolFactory _protocolFactory{};

    public:
        IpcServer(ProtocolFactory protocolFactory)
            : _protocolFactory(protocolFactory)
        {
        }

        virtual ~IpcServer() = default;
        virtual void setup() = 0;
        virtual void run() = 0;
        virtual void stop() = 0;
    };
} // namespace ipc