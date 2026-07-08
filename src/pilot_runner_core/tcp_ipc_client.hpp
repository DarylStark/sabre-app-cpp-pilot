#include "ipc_client.hpp"
#include <asio.hpp>
#include <cstdint>
#include <deque>
#include <memory>
#include <string>

namespace sabre_pilot_runner_core
{
    class TcpIpcClient : public IpcClient,
                         public std::enable_shared_from_this<TcpIpcClient>
    {
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

    public:
        TcpIpcClient(const std::string &serverIp, uint16_t serverPort);

        void setup() override;

        void start() override;
        void stop() override;

        bool waitForConnection() override;

        void sendData(const std::string &data) override;
    };
} // namespace sabre_pilot_runner_core