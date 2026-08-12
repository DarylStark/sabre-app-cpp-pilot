#pragma once

#include <asio.hpp>
#include <deque>
#include <ipc/client.hpp>
#include <ipc/protocol.hpp>
#include <string>
#include <vector>

namespace ipc
{
    class TcpIpcClient : public IpcClient,
                         public std::enable_shared_from_this<TcpIpcClient>
    {
        using Protocol = IpcProtocol;
        using std::enable_shared_from_this<TcpIpcClient>::shared_from_this;

    private:
        const std::string _serverIp;
        const uint16_t _serverPort;

        asio::io_context _ioContext;
        asio::ip::tcp::resolver _resolver;
        asio::ip::tcp::socket _socket;

        std::array<std::uint8_t, 4096> _readBuffer{};
        std::deque<std::vector<std::uint8_t>> _writeQueue;

        std::mutex _connectionMutex;
        std::condition_variable _connectionCv;
        bool _isConnected = false;
        bool _connectionFailed = false;

        void _writeNext();
        void _startRead();

        void _callbackAsyncReadSome(const std::error_code &ec,
                                    std::size_t bytesTransferred);
        void _callbackAsyncResolve(
            const std::error_code &ec,
            const asio::ip::tcp::resolver::results_type &endpoints);

    public:
        TcpIpcClient(std::shared_ptr<Protocol> protocol,
                     const std::string &serverAddress, uint16_t serverPort);
        void setup() override;
        void run() override;
        void stop() override;

        bool waitForConnection() override;
        void sendData(const std::string &data) override;
    };

} // namespace ipc