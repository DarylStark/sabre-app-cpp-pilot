#include "tcp_ipc_client.hpp"
#include <asio.hpp>

namespace sabre_pilot_runner_core
{
    TcpIpcClient::TcpIpcClient(const std::string &serverIp, uint16_t port)
        : _serverIp(serverIp), _serverPort(port), _resolver(_ioContext),
          _socket(_ioContext)
    {
    }

    void TcpIpcClient::connect()
    {
        auto endpoints =
            _resolver.resolve(_serverIp, std::to_string(_serverPort));
        asio::connect(_socket, endpoints);
    }

    void TcpIpcClient::disconnect()
    {
        if (!_socket.is_open())
        {
            return;
        }

        std::error_code ec;
        _socket.shutdown(asio::ip::tcp::socket::shutdown_both, ec);
        _socket.close();
    }

    void TcpIpcClient::sendData(const std::string &data)
    {
        asio::write(_socket, asio::buffer(data));
    }

} // namespace sabre_pilot_runner_core