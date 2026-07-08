#pragma once

#include <array>
#include <asio.hpp>
#include <deque>
#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace sabre_pilot
{

    class TcpSession : public std::enable_shared_from_this<TcpSession>
    {
    private:
        using DisconnectHandler =
            std::function<void(std::shared_ptr<TcpSession>)>;
        using ReceiveHandler = std::function<void(
            std::shared_ptr<TcpSession>, const std::vector<std::uint8_t> &)>;

        asio::ip::tcp::socket _socket;
        std::array<std::uint8_t, 4096> _readBuffer{};
        std::deque<std::vector<std::uint8_t>> _writeQueue;

        DisconnectHandler _disconnectHandler;
        ReceiveHandler _receiveHandler;

        bool _stopped = false;

        void _readSome();
        void _writeNext();
        void _handleDisconnect();

    public:
        explicit TcpSession(asio::ip::tcp::socket socket);

        void start();
        void stop();

        void send(const std::vector<std::uint8_t> &data);
        void send(std::string_view text);

        void setDisconnectHandler(DisconnectHandler handler);
        void setReceiveHandler(ReceiveHandler handler);
    };
} // namespace sabre_pilot