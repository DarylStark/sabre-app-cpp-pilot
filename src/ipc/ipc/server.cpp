#include "server.hpp"

namespace sabre_pilot::ipc
{
    IpcServer(std::shared_ptr<IpcProtocol> protocol) : _protocol(protocol) {}
} // namespace sabre_pilot::ipc