#pragma once

#include "ipc_command.hpp"
#include <memory>
#include <pilot_ipc_server/ipc_protocol.hpp>
#include <queue>

namespace sabre_pilot::ipc
{
    class PilotIpcProtocol : public sabre_pilot::IpcProtocol
    {
    protected:
        std::queue<std::unique_ptr<sabre_pilot::ipc::IpcCommand>> &_queue;

    public:
        PilotIpcProtocol(
            std::queue<std::unique_ptr<sabre_pilot::ipc::IpcCommand>> &queue);
    };
} // namespace sabre_pilot::ipc