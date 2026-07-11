#pragma once

#include "ipc_protocol.hpp"
#include <memory>

namespace sabre_pilot
{
    class IpcServer
    {
    protected:
        std::shared_ptr<IpcProtocol> _protocol;

    public:
        IpcServer(std::shared_ptr<IpcProtocol> _protocol);
        virtual ~IpcServer() = default;
        virtual void setup() = 0;
        virtual void start() = 0;
        virtual void stop() = 0;
    };
} // namespace sabre_pilot