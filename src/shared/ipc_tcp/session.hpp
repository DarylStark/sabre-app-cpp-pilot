#pragma once

#include <array>
#include <asio.hpp>
#include <deque>
#include <functional>
#include <ipc/protocol.hpp>
#include <ipc/session.hpp>
#include <memory>
#include <string>
#include <vector>

namespace ipc::tcp
{
    class TcpIpcSession : public ::ipc::IpcSession,
                          public std::enable_shared_from_this<TcpIpcSession>
    {
        using Protocol = IpcProtocol;
        using std::enable_shared_from_this<TcpIpcSession>::shared_from_this;

    private:
        asio::ip::tcp::socket _socket;
        std::array<std::byte, 4096> _readBuffer{};
        std::deque<std::vector<std::uint8_t>> _writeQueue;

        ::ipc::IpcSession::DisconnectHandler _disconnectHandler;

        bool _stopped = false;

        void _readSome();
        void _writeNext();
        void _handleDisconnect();

        bool _stopOnError(const std::error_code &ec);

        void _callbackAsyncReadSome(const std::error_code &ec,
                                    std::size_t bytesTransferred);
        void _callbackAsyncWrite(const std::error_code &ec, std::size_t size);

    public:
        TcpIpcSession(asio::ip::tcp::socket socket);

        void start();
        void stop();

        void send(const std::vector<std::uint8_t> &data);

        void setDisconnectHandler(::ipc::IpcSession::DisconnectHandler handler);
    };
} // namespace ipc::tcp
