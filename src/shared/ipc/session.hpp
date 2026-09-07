#pragma once

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

    public:
        virtual void start() = 0;
        virtual void stop() = 0;

        virtual void send(const std::vector<std::uint8_t> &data) = 0;
        virtual void send(std::string_view text) = 0;

        virtual void setDisconnectHandler(DisconnectHandler handler) = 0;
    };
} // namespace ipc