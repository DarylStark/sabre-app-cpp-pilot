#include "ipc_protocol.hpp"

namespace sabre_pilot::ipc
{
    PilotIpcProtocol::PilotIpcProtocol(
        std::queue<std::unique_ptr<sabre_pilot::ipc::IpcCommand>> &queue)
        : _queue(queue)
    {
    }
} // namespace sabre_pilot::ipc