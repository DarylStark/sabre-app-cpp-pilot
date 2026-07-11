#include "ipc_server.hpp"

namespace sabre_pilot
{
    IpcServer::IpcServer(std::shared_ptr<IpcProtocol> protocol)
        : _protocol(std::move(protocol))
    {
    }
} // namespace sabre_pilot