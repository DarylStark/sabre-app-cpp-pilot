#include "tcp_ipc_server.hpp"
#include "tcp_session.hpp"
#include <iostream>

namespace sabre_pilot
{
    using asio::ip::tcp;

    TcpIpcServer::TcpIpcServer(uint16_t port,
                               std::shared_ptr<IpcProtocol> protocol)
        : IpcServer(std::move(protocol)),
          _acceptor(_io_context, asio::ip::tcp::endpoint(tcp::v4(), port)),
          _port(port)
    {
    }

    void TcpIpcServer::_callbackAsyncAccept(const std::error_code &ec,
                                            asio::ip::tcp::socket socket)
    {
        if (!ec)
        {
            std::cout << "SERVER: New connection has been made!\n";
            std::cout << "SERVER: Origin: " << socket.remote_endpoint() << '\n';

            auto session =
                std::make_shared<TcpSession>(std::move(socket), _protocol);

            session->setDisconnectHandler(
                [this](const std::shared_ptr<TcpSession> &sessionToRemove)
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

    void TcpIpcServer::_configureAcceptCallback()
    {
        _acceptor.async_accept(
            [this](std::error_code ec, tcp::socket socket)
            { _callbackAsyncAccept(ec, std::move(socket)); });
    }

    void TcpIpcServer::setup()
    {
        std::cout << "SERVER: Setting up TCP IPC server on port " << _port
                  << '\n';
        _configureAcceptCallback();
    }

    void TcpIpcServer::start()
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

    void TcpIpcServer::broadcast(std::string_view text)
    {
        for (auto &session : _sessions)
        {
            session->send(text);
        }
    }

    void
    TcpIpcServer::_removeSession(const std::shared_ptr<TcpSession> &session)
    {
        std::erase(_sessions, session);
        std::cout << "SERVER: Session removed. Active sessions: "
                  << _sessions.size() << '\n';
    }
} // namespace sabre_pilot