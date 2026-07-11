#include "ipc_protocol.hpp"

namespace sabre_pilot::ipc
{
    PilotIpcProtocol::PilotIpcProtocol(
        std::queue<std::shared_ptr<sabre_pilot::ipc::IpcMessage>> &queue)
        : _queue(queue)
    {
    }
} // namespace sabre_pilot::ipc