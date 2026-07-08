#include "tcp_ipc_server.hpp"
#include "tcp_session.hpp"
#include <iostream>

namespace sabre_pilot
{
    using asio::ip::tcp;

    TcpIpcServer::TcpIpcServer(uint16_t port)
        : _acceptor(_io_context, asio::ip::tcp::endpoint(tcp::v4(), port)),
          _port(port)
    {
    }

    void TcpIpcServer::_configureAcceptCallback()
    {
        _acceptor.async_accept(
            [this](std::error_code ec, tcp::socket socket)
            {
                if (!ec)
                {
                    std::cout << "New connection has been made!\n";
                    std::cout << "Origin: " << socket.remote_endpoint() << '\n';

                    auto session =
                        std::make_shared<TcpSession>(std::move(socket));

                    session->setDisconnectHandler(
                        [this](
                            const std::shared_ptr<TcpSession> &sessionToRemove)
                        { _removeSession(sessionToRemove); });

                    session->setReceiveHandler(
                        [this](const std::shared_ptr<TcpSession> &session,
                               const std::vector<std::uint8_t> &data)
                        { _handleMessage(session, data); });

                    _sessions.push_back(session);
                    session->start();
                }
                else
                {
                    std::cerr << "Accept error: " << ec.message() << '\n';
                }

                this->_configureAcceptCallback();
            });
    }

    void TcpIpcServer::setup()
    {
        std::cout << "Setting up TCP IPC server on port " << _port << '\n';
        _configureAcceptCallback();
    }

    void TcpIpcServer::start()
    {
        std::cout << "Running TcpIpcServer on port " << _port << '\n';
        _io_context.run();
    }

    void TcpIpcServer::stop()
    {
        std::cout << "Stopping sessions for TcpIpcServer\n";
        for (auto &session : _sessions)
        {
            session->stop();
        }
        _sessions.clear();

        std::cout << "Stopping TcpIpcServer\n";
        _io_context.stop();

        std::cout << "TcpIpcServer stopped\n";
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
        std::cout << "Session removed. Active sessions: " << _sessions.size()
                  << '\n';
    }

    void
    TcpIpcServer::_handleMessage(const std::shared_ptr<TcpSession> &session,
                                 const std::vector<std::uint8_t> &data)
    {
        std::string text(data.begin(), data.end());
        std::cout << "Received:\n\n" << text << "\n\n";
        std::cout << "This was " << data.size() << " bytes\n\n";

        // Example response
        session->send("ACK: " + text);
    }
} // namespace sabre_pilot