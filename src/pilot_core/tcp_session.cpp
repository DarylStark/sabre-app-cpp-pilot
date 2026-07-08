#include "tcp_session.hpp"
#include <iostream>

namespace sabre_pilot
{
    using asio::ip::tcp;

    TcpSession::TcpSession(tcp::socket socket) : _socket(std::move(socket)) {}

    void TcpSession::start()
    {
        std::cout << "Session started: " << _socket.remote_endpoint() << '\n';
        _readSome();
    }

    void TcpSession::stop()
    {
        if (_stopped)
        {
            return;
        }

        _stopped = true;
        std::error_code ec;
        _socket.shutdown(tcp::socket::shutdown_both, ec);
        _socket.close(ec);

        _handleDisconnect();
    }

    void TcpSession::send(const std::vector<std::uint8_t> &data)
    {
        bool write_in_progress = !_writeQueue.empty();
        _writeQueue.push_back(data);
        if (!write_in_progress)
        {
            _writeNext();
        }
    }

    void TcpSession::send(std::string_view text)
    {
        send(std::vector<std::uint8_t>(text.begin(), text.end()));
    }

    void TcpSession::setDisconnectHandler(DisconnectHandler handler)
    {
        _disconnectHandler = std::move(handler);
    }

    void TcpSession::setReceiveHandler(ReceiveHandler handler)
    {
        _receiveHandler = std::move(handler);
    }

    void TcpSession::_readSome()
    {
        auto self = shared_from_this();

        _socket.async_read_some(
            asio::buffer(_readBuffer),
            [this, self](const std::error_code &ec,
                         std::size_t bytesTransferred)
            {
                if (ec)
                {
                    std::cerr << "Red error: " << ec.message() << '\n';
                    stop();
                    return;
                }

                if (bytesTransferred > _readBuffer.size())
                {
                    throw std::runtime_error("bytesTransferred exceeds read "
                                             "buffer size");
                }

                std::vector<std::uint8_t> data(
                    _readBuffer.begin(),
                    _readBuffer.begin() +
                        static_cast<std::ptrdiff_t>(bytesTransferred));

                if (_receiveHandler)
                {
                    _receiveHandler(self, data);
                }

                _readSome();
            });
    }

    void TcpSession::_writeNext()
    {
        if (_writeQueue.empty() || _stopped)
        {
            return;
        }

        auto self = shared_from_this();

        asio::async_write(_socket, asio::buffer(_writeQueue.front()),
                          [this, self](const std::error_code &ec, std::size_t)
                          {
                              if (ec)
                              {
                                  std::cerr << "Write error: " << ec.message()
                                            << '\n';
                                  stop();
                                  return;
                              }

                              _writeQueue.pop_front();

                              if (!_writeQueue.empty())
                              {
                                  _writeNext();
                              }
                          });
    }

    void TcpSession::_handleDisconnect()
    {
        if (_disconnectHandler)
        {
            _disconnectHandler(shared_from_this());
        }
    }
} // namespace sabre_pilot