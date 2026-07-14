#pragma once

#include "../ipc/protocol.hpp"
#include "../ipc/server.hpp"
#include <iostream> // TODO: Remove
#include <thread>   // TODO: Remove

namespace ipc
{
    template <typename MessageType>
    class TcpIpcServer : public IpcServer<MessageType>
    {
    private:
        uint16_t _port;

    public:
        TcpIpcServer(std::shared_ptr<IpcProtocol<MessageType>> protocol,
                     uint16_t port);
        void setup() override;
        void run() override;
        void stop() override;
    };

    template <typename MessageType>
    TcpIpcServer<MessageType>::TcpIpcServer(
        std::shared_ptr<IpcProtocol<MessageType>> protocol, uint16_t port)
        : IpcServer<MessageType>(protocol), _port(port)
    {
    }

    template <typename MessageType>
    void TcpIpcServer<MessageType>::setup()
    {
        std::cout << "Setting up everything\n";
    }

    template <typename MessageType>
    void TcpIpcServer<MessageType>::run()
    {
        while (true)
        {
            using namespace std::chrono_literals;
            std::cout << "HIIII\n";
            std::this_thread::sleep_for(1s);
        }
    }

    template <typename MessageType>
    void TcpIpcServer<MessageType>::stop()
    {
    }
} // namespace ipc