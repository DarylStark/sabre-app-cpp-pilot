#include "ipc_client.hpp"
#include <asio.hpp>
#include <cstdint>
#include <string>

namespace sabre_pilot_runner_core
{
    class TcpIpcClient : public IpcClient
    {
    private:
        const std::string _serverIp;
        const uint16_t _serverPort;

        asio::io_context _ioContext;
        asio::ip::tcp::resolver _resolver;
        asio::ip::tcp::socket _socket;

    public:
        TcpIpcClient(const std::string &serverIp, uint16_t serverPort);
        void connect() override;
        void disconnect() override;
        void sendData(const std::string &data) override;
    };
} // namespace sabre_pilot_runner_core