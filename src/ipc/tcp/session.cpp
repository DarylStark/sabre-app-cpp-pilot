#include "session.hpp"
#include <iostream>

namespace ipc
{
    TcpIpcSession::TcpIpcSession(asio::ip::tcp::socket socket,
                                 std::unique_ptr<Protocol> protocol)
        : _socket(std::move(socket)), _protocol(std::move(protocol))
    {
    }

    void TcpIpcSession::start()
    {
        std::cout << "SESSION: Session started: " << _socket.remote_endpoint()
                  << '\n';
        _readSome();
    }

    void TcpIpcSession::stop()
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

    void TcpIpcSession::send(const std::vector<std::uint8_t> &data)
    {
        bool write_in_progress = !_writeQueue.empty();
        _writeQueue.push_back(data);
        if (!write_in_progress)
        {
            _writeNext();
        }
    }

    void TcpIpcSession::send(std::string_view text)
    {
        send(std::vector<std::uint8_t>(text.begin(), text.end()));
    }

    void TcpIpcSession::setDisconnectHandler(DisconnectHandler handler)
    {
        _disconnectHandler = std::move(handler);
    }

    bool TcpIpcSession::_stopOnError(const std::error_code &ec)
    {
        if (ec)
        {
            std::cerr << "SESSION: Read error: " << ec.message() << '\n';
            stop();
            return true;
        }
        return false;
    }

    void TcpIpcSession::_callbackAsyncReadSome(const std::error_code &ec,
                                               std::size_t bytesTransferred)
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

    void TcpIpcSession::_readSome()
    {
        auto self = shared_from_this();

        _socket.async_read_some(
            asio::buffer(_readBuffer),
            [this, self](const std::error_code &ec,
                         std::size_t bytesTransferred)
            { _callbackAsyncReadSome(ec, bytesTransferred); });
    }

    void TcpIpcSession::_callbackAsyncWrite(const std::error_code &ec,
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

    void TcpIpcSession::_writeNext()
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

    void TcpIpcSession::_handleDisconnect()
    {
        if (_disconnectHandler)
        {
            _disconnectHandler(shared_from_this());
        }
    }
} // namespace ipc