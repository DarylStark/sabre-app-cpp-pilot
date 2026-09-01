#include "client.hpp"
#include <iostream>

namespace ipc::tcp
{
    TcpIpcClient::TcpIpcClient(std::shared_ptr<Protocol> protocol,
                               const std::string &serverAddress,
                               uint16_t serverPort)
        : IpcClient(std::move(protocol)), _serverIp(serverAddress),
          _serverPort(serverPort), _resolver(_ioContext), _socket(_ioContext)
    {
    }

    void TcpIpcClient::_writeNext()
    {
        if (_writeQueue.empty())
        {
            return;
        }

        auto self = shared_from_this();

        asio::async_write(_socket, asio::buffer(_writeQueue.front()),
                          [this, self](const std::error_code &ec, std::size_t)
                          {
                              if (ec)
                              {
                                  std::cerr << "Write failed: " << ec.message()
                                            << '\n';
                                  return;
                              }

                              _writeQueue.pop_front();

                              if (!_writeQueue.empty())
                              {
                                  _writeNext();
                              }
                          });
    }

    void TcpIpcClient::_startRead()
    {
        auto self = shared_from_this();

        _socket.async_read_some(
            asio::buffer(_readBuffer),
            [this, self](const std::error_code &ec,
                         std::size_t bytesTransferred)
            { _callbackAsyncReadSome(ec, bytesTransferred); });
    }

    void TcpIpcClient::_callbackAsyncReadSome(const std::error_code &ec,
                                              std::size_t bytesTransferred)
    {
        if (ec)
        {
            std::cerr << "Read failed: " << ec.message() << '\n';
            return;
        }

        std::vector<std::uint8_t> data(_readBuffer.begin(),
                                       _readBuffer.begin() + bytesTransferred);
        std::string strData(data.begin(), data.end());

        std::cout << "CLIENT: Received: " << strData;
        std::cout << "CLIENT: This was " << data.size() << " bytes\n";

        _startRead();
    }

    void TcpIpcClient::_callbackAsyncResolve(
        const std::error_code &ec,
        const asio::ip::tcp::resolver::results_type &endpoints)
    {
        if (ec)
        {
            {
                std::lock_guard<std::mutex> lock(_connectionMutex);
                _connectionFailed = true;
            }

            _connectionCv.notify_all();

            std::cerr << "Resolve failed: " << ec.message() << '\n';
            return;
        }

        auto self = shared_from_this();

        asio::async_connect(
            _socket, endpoints,
            [this, self](const std::error_code &ec,
                         const asio::ip::tcp::endpoint &)
            {
                {
                    std::lock_guard<std::mutex> lock(_connectionMutex);

                    if (ec)
                    {
                        _connectionFailed = true;
                    }
                    else
                    {
                        _isConnected = true;
                    }
                }

                _connectionCv.notify_all();

                if (ec)
                {
                    std::cerr << "Connect failed: " << ec.message() << '\n';
                    return;
                }

                std::cout << "CLIENT: Connected to server\n";
                _startRead();
            });
    }

    void TcpIpcClient::setup()
    {
        auto self = shared_from_this();

        _resolver.async_resolve(
            _serverIp, std::to_string(_serverPort),
            [this, self](const std::error_code &ec,
                         const asio::ip::tcp::resolver::results_type &endpoints)
            { _callbackAsyncResolve(ec, endpoints); });
    }

    void TcpIpcClient::run()
    {
        _ioContext.run();
    }

    void TcpIpcClient::stop()
    {
        if (!_socket.is_open())
        {
            return;
        }

        std::error_code ec;
        _socket.shutdown(asio::ip::tcp::socket::shutdown_both, ec);
        _socket.close();

        _ioContext.stop();
    }

    bool TcpIpcClient::waitForConnection()
    {
        std::unique_lock<std::mutex> lock(_connectionMutex);

        _connectionCv.wait(lock, [this]()
                           { return _isConnected || _connectionFailed; });

        return _isConnected;
    }

    void TcpIpcClient::sendData(const BufferType &data)
    {
        ::ipc::BufferType bytesData(data.begin(), data.end());
        bool writeInProgress = !_writeQueue.empty();
        _writeQueue.push_back(bytesData);

        if (!writeInProgress)
        {
            _writeNext();
        }
    }
} // namespace ipc::tcp