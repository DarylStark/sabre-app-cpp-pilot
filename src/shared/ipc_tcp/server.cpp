#include "server.hpp"
#include <iostream>

namespace ipc::tcp
{
    TcpIpcServer::TcpIpcServer(::ipc::ProtocolFactory protocolFactory,
                               uint16_t port)
        : IpcServer(std::move(protocolFactory)), _port(port),
          _acceptor(_io_context,
                    asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port))
    {
    }

    void TcpIpcServer::_removeSession(const std::shared_ptr<Session> &session)
    {
        std::erase(_sessions, session);
        std::cout << "SERVER: Session removed. Active sessions: "
                  << _sessions.size() << '\n';
    }

    void TcpIpcServer::_configureAcceptCallback()
    {
        _acceptor.async_accept(
            [this](std::error_code ec, asio::ip::tcp::socket socket)
            { _callbackAsyncAccept(ec, std::move(socket)); });
    }

    void TcpIpcServer::_callbackAsyncAccept(const std::error_code &ec,
                                            asio::ip::tcp::socket socket)
    {
        if (!ec)
        {
            std::cout << "SERVER: New connection has been made!\n";
            std::cout << "SERVER: Origin: " << socket.remote_endpoint() << '\n';

            auto session = std::make_shared<Session>(std::move(socket),
                                                     this->_protocolFactory());

            session->setDisconnectHandler(
                [this](const std::shared_ptr<Session> &sessionToRemove)
                { _removeSession(sessionToRemove); });

            _sessions.push_back(session);
            session->start();
        }
        else
        {
            std::cerr << "Accept error: " << ec.message() << '\n';
        }

        this->_configureAcceptCallback();
    }

    void TcpIpcServer::setup()
    {
        std::cout << "SERVER: Setting up TCP IPC server on port " << _port
                  << '\n';
        _configureAcceptCallback();
    }

    void TcpIpcServer::run()
    {
        std::cout << "SERVER: Running TcpIpcServer on port " << _port << '\n';
        _io_context.run();
    }

    void TcpIpcServer::stop()
    {
        std::cout << "SERVER: Stopping sessions for TcpIpcServer\n";
        for (auto &session : _sessions)
        {
            session->stop();
        }
        _sessions.clear();

        std::cout << "SERVER: Stopping TcpIpcServer\n";
        _io_context.stop();

        std::cout << "SERVER: TcpIpcServer stopped\n";
    }
} // namespace ipc::tcp