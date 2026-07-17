#pragma once

#include "session.hpp"
#include <asio.hpp>
#include <iostream> // TODO: Remove
#include <ipc/protocol.hpp>
#include <ipc/queue.hpp>
#include <ipc/server.hpp>
#include <memory>
#include <thread> // TODO: Remove
#include <vector>

namespace ipc
{
    template <typename MessageType>
    class TcpIpcServer : public IpcServer<MessageType>
    {
        using Session = TcpIpcSession<MessageType>;
        using Protocol = IpcProtocol<MessageType>;
        using IpcServer<MessageType>::_protocol;
        using IpcServer<MessageType>::_queue;

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
        TcpIpcServer(std::shared_ptr<Protocol> protocol,
                     Queue<MessageType> &queue, uint16_t port);
        void setup() override;
        void run() override;
        void stop() override;
    };

    template <typename MessageType>
    TcpIpcServer<MessageType>::TcpIpcServer(std::shared_ptr<Protocol> protocol,
                                            Queue<MessageType> &queue,
                                            uint16_t port)
        : IpcServer<MessageType>(protocol, queue), _port(port),
          _acceptor(_io_context,
                    asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port))
    {
    }

    template <typename MessageType>
    void TcpIpcServer<MessageType>::_removeSession(
        const std::shared_ptr<Session> &session)
    {
        std::erase(_sessions, session);
        std::cout << "SERVER: Session removed. Active sessions: "
                  << _sessions.size() << '\n';
    }

    template <typename MessageType>
    void TcpIpcServer<MessageType>::_configureAcceptCallback()
    {
        _acceptor.async_accept(
            [this](std::error_code ec, asio::ip::tcp::socket socket)
            { _callbackAsyncAccept(ec, std::move(socket)); });
    }

    template <typename MessageType>
    void TcpIpcServer<MessageType>::_callbackAsyncAccept(
        const std::error_code &ec, asio::ip::tcp::socket socket)
    {
        if (!ec)
        {
            std::cout << "SERVER: New connection has been made!\n";
            std::cout << "SERVER: Origin: " << socket.remote_endpoint() << '\n';

            auto session = std::make_shared<Session>(
                std::move(socket), this->_protocol->clone(), _queue);

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

    template <typename MessageType>
    void TcpIpcServer<MessageType>::setup()
    {
        std::cout << "SERVER: Setting up TCP IPC server on port " << _port
                  << '\n';
        _configureAcceptCallback();
    }

    template <typename MessageType>
    void TcpIpcServer<MessageType>::run()
    {
        std::cout << "SERVER: Running TcpIpcServer on port " << _port << '\n';
        _io_context.run();
    }

    template <typename MessageType>
    void TcpIpcServer<MessageType>::stop()
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
} // namespace ipc