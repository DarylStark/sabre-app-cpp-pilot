#include "ipc_server.hpp"
#include "tcp_session.hpp"
#include <asio.hpp>
#include <vector>

namespace sabre_pilot
{
    class TcpIpcServer : public IpcServer
    {
    private:
        void _configureAcceptCallback();
        void _removeSession(const std::shared_ptr<TcpSession> &session);
        void _handleMessage(const std::shared_ptr<TcpSession> &session,
                            const std::vector<std::uint8_t> &data);

        asio::io_context _io_context;
        asio::ip::tcp::acceptor _acceptor;
        std::vector<std::shared_ptr<TcpSession>> _sessions;

        uint16_t _port;

    public:
        explicit TcpIpcServer(uint16_t port);
        void setup() override;
        void start() override;
        void stop() override;

        void broadcast(std::string_view text); // TODO: add this to interface?
    };
} // namespace sabre_pilot