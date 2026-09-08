#pragma once

#include "protocol.hpp"
#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace ipc
{
    class IpcSession
    {
    public:
        using DisconnectHandler =
            std::function<void(std::shared_ptr<IpcSession>)>;

    protected:
        std::unique_ptr<IpcProtocol> _protocol{};

    public:
        virtual void start() = 0;
        virtual void stop() = 0;

        void setProtocol(std::unique_ptr<IpcProtocol> protocol);

        // TODO: Make this accept a range instead of a vector
        virtual void send(const std::vector<std::uint8_t> &data) = 0;

        // TODO: move this back to TcpIpcSession; is not a global method
        virtual void setDisconnectHandler(DisconnectHandler handler) = 0;
    };
} // namespace ipc