#include "tcp_session.hpp"
#include <asio.hpp>
#include <pilot_ipc_protocol/ipc_message.hpp>
#include <pilot_ipc_server/ipc_server.hpp>
#include <vector>

namespace sabre_pilot
{
    class TcpIpcServer : public IpcServer
    {
    private:
        void _configureAcceptCallback();
        void _removeSession(const std::shared_ptr<TcpSession> &session);

        asio::io_context _io_context;
        asio::ip::tcp::acceptor _acceptor;
        std::vector<std::shared_ptr<TcpSession>> _sessions;

        uint16_t _port;

        void _callbackAsyncAccept(const std::error_code &ec,
                                  asio::ip::tcp::socket socket);

    public:
        explicit TcpIpcServer(uint16_t port,
                              std::shared_ptr<IpcProtocol> protocol);
        void setup() override;
        void start() override;
        void stop() override;

        void broadcast(std::string_view text); // TODO: add this to interface?
    };
} // namespace sabre_pilot