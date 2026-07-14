#include "client.hpp"

namespace sabre_pilot::ipc
{
    IpcClient(std::shared_ptr<IpcProtocol> protocol) : _protocol(protocol) {}
} // namespace sabre_pilot::ipc