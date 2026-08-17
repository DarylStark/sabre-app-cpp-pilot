#pragma once

#include "session.hpp"
#include <asio.hpp>
#include <functional>
#include <ipc/protocol.hpp>
#include <ipc/queue.hpp>
#include <ipc/server.hpp>
#include <memory>
#include <vector>

namespace ipc::tcp
{
    class TcpIpcServer : public IpcServer
    {
        using Session = TcpIpcSession;
        using Protocol = IpcProtocol;

    private:
        uint16_t _port;
        asio::io_context _io_context;
        asio::ip::tcp::acceptor _acceptor;
        std::vector<std::shared_ptr<Session>> _sessions;

        void _configureAcceptCallback();
        void _removeSession(const std::shared_ptr<Session> &session);
        void _callbackAsyncAccept(const std::error_code &ec,
                                  asio::ip::tcp::socket socket);

    public:
        TcpIpcServer(::ipc::ProtocolFactory protocolFactory, uint16_t port);
        void setup() override;
        void run() override;
        void stop() override;
    };

} // namespace ipc::tcp