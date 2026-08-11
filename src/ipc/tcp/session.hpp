#pragma once

#include <array>
#include <asio.hpp>
#include <deque>
#include <functional>
#include <ipc/protocol.hpp>
#include <ipc/queue.hpp>
#include <memory>
#include <string>
#include <vector>

namespace ipc
{
    template <typename MessageType>
    class TcpIpcSession
        : public std::enable_shared_from_this<TcpIpcSession<MessageType>>
    {
        using Protocol = IpcProtocol<MessageType>;
        using std::enable_shared_from_this<
            TcpIpcSession<MessageType>>::shared_from_this;

    private:
        using DisconnectHandler =
            std::function<void(std::shared_ptr<TcpIpcSession>)>;

        asio::ip::tcp::socket _socket;
        std::array<std::uint8_t, 4096> _readBuffer{};
        std::deque<std::vector<std::uint8_t>> _writeQueue;

        DisconnectHandler _disconnectHandler;

        std::unique_ptr<Protocol> _protocol;
        Queue<MessageType> &_queue;

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
                      std::unique_ptr<Protocol> protocol,
                      Queue<MessageType> &queue);

        void start();
        void stop();

        void send(const std::vector<std::uint8_t> &data);
        void send(std::string_view text);

        void setDisconnectHandler(DisconnectHandler handler);
    };

    template <typename MessageType>
    TcpIpcSession<MessageType>::TcpIpcSession(
        asio::ip::tcp::socket socket, std::unique_ptr<Protocol> protocol,
        Queue<MessageType> &queue)
        : _socket(std::move(socket)), _protocol(std::move(protocol)),
          _queue(queue)
    {
    }

    template <typename MessageType>
    void TcpIpcSession<MessageType>::start()
    {
        std::cout << "SESSION: Session started: " << _socket.remote_endpoint()
                  << '\n';
        _readSome();
    }

    template <typename MessageType>
    void TcpIpcSession<MessageType>::stop()
    {
        if (_stopped)
        {
            return;
        }

        _stopped = true;
        std::error_code ec;
        _socket.shutdown(asio::ip::tcp::socket::shutdown_both, ec);
        _socket.close(ec);

        _handleDisconnect();
    }

    template <typename MessageType>
    void TcpIpcSession<MessageType>::send(const std::vector<std::uint8_t> &data)
    {
        bool write_in_progress = !_writeQueue.empty();
        _writeQueue.push_back(data);
        if (!write_in_progress)
        {
            _writeNext();
        }
    }

    template <typename MessageType>
    void TcpIpcSession<MessageType>::send(std::string_view text)
    {
        send(std::vector<std::uint8_t>(text.begin(), text.end()));
    }

    template <typename MessageType>
    void
    TcpIpcSession<MessageType>::setDisconnectHandler(DisconnectHandler handler)
    {
        _disconnectHandler = std::move(handler);
    }

    template <typename MessageType>
    bool TcpIpcSession<MessageType>::_stopOnError(const std::error_code &ec)
    {
        if (ec)
        {
            std::cerr << "SESSION: Read error: " << ec.message() << '\n';
            stop();
            return true;
        }
        return false;
    }

    template <typename MessageType>
    void TcpIpcSession<MessageType>::_callbackAsyncReadSome(
        const std::error_code &ec, std::size_t bytesTransferred)
    {
        if (_stopOnError(ec))
        {
            return;
        }

        if (bytesTransferred > _readBuffer.size())
        {
            throw std::runtime_error("bytesTransferred exceeds read "
                                     "buffer size");
        }

        _readSome();

        std::vector<std::uint8_t> data(
            _readBuffer.begin(),
            _readBuffer.begin() +
                static_cast<std::ptrdiff_t>(bytesTransferred));

        std::cout << "Received data: " << data.size() << " bytes\n";
        _protocol->parseBytes(data);
    }

    template <typename MessageType>
    void TcpIpcSession<MessageType>::_readSome()
    {
        auto self = shared_from_this();

        _socket.async_read_some(
            asio::buffer(_readBuffer),
            [this, self](const std::error_code &ec,
                         std::size_t bytesTransferred)
            { _callbackAsyncReadSome(ec, bytesTransferred); });
    }

    template <typename MessageType>
    void
    TcpIpcSession<MessageType>::_callbackAsyncWrite(const std::error_code &ec,
                                                    std::size_t size)
    {
        if (_stopOnError(ec))
        {
            return;
        }

        _writeQueue.pop_front();

        if (!_writeQueue.empty())
        {
            _writeNext();
        }
    }

    template <typename MessageType>
    void TcpIpcSession<MessageType>::_writeNext()
    {
        if (_writeQueue.empty() || _stopped)
        {
            return;
        }

        auto self = shared_from_this();

        asio::async_write(
            _socket, asio::buffer(_writeQueue.front()),
            [this, self](const std::error_code &ec, std::size_t size)
            { _callbackAsyncWrite(ec, size); });
    }

    template <typename MessageType>
    void TcpIpcSession<MessageType>::_handleDisconnect()
    {
        if (_disconnectHandler)
        {
            _disconnectHandler(shared_from_this());
        }
    }
} // namespace ipc
