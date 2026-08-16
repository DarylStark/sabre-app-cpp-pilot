#pragma once

#include <array>
#include <asio.hpp>
#include <deque>
#include <functional>
#include <ipc/protocol.hpp>
#include <memory>
#include <string>
#include <vector>

namespace ipc
{
    class TcpIpcSession : public std::enable_shared_from_this<TcpIpcSession>
    {
        using Protocol = IpcProtocol;
        using std::enable_shared_from_this<TcpIpcSession>::shared_from_this;

    private:
        using DisconnectHandler =
            std::function<void(std::shared_ptr<TcpIpcSession>)>;

        asio::ip::tcp::socket _socket;
        std::array<std::uint8_t, 4096> _readBuffer{};
        std::deque<std::vector<std::uint8_t>> _writeQueue;

        DisconnectHandler _disconnectHandler;

        std::unique_ptr<Protocol> _protocol;

        bool _stopped = false;

        void _readSome();
        void _writeNext();
        void _handleDisconnect();

        bool _stopOnError(const std::error_code &ec);

        void _callbackAsyncReadSome(const std::error_code &ec,
                                    std::size_t bytesTransferred);
        void _callbackAsyncWrite(const std::error_code &ec, std::size_t size);

    public:
        TcpIpcSession(asio::ip::tcp::socket socket,
                      std::unique_ptr<Protocol> protocol);

        void start();
        void stop();

        void send(const std::vector<std::uint8_t> &data);
        void send(std::string_view text);

        void setDisconnectHandler(DisconnectHandler handler);
    };
} // namespace ipc
