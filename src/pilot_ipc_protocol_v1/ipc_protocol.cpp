#include "ipc_protocol.hpp"

namespace sabre_pilot::ipc
{
    PilotIpcProtocol_v1::PilotIpcProtocol_v1(
        std::queue<std::unique_ptr<sabre_pilot::ipc::IpcCommand>> &queue)
        : sabre_pilot::ipc::PilotIpcProtocol(queue)
    {
    }

    void PilotIpcProtocol_v1::processData()
    {
        return;
    }
} // namespace sabre_pilot::ipc