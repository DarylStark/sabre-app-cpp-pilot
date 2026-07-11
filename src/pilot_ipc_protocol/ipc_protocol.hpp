#pragma once

#include "ipc_message.hpp"
#include <memory>
#include <pilot_ipc_server/ipc_protocol.hpp>
#include <queue>

namespace sabre_pilot::ipc
{
    class PilotIpcProtocol : public sabre_pilot::IpcProtocol
    {
    protected:
        std::queue<std::shared_ptr<sabre_pilot::ipc::IpcMessage>> &_queue;

    public:
        PilotIpcProtocol(
            std::queue<std::shared_ptr<sabre_pilot::ipc::IpcMessage>> &queue);
    };
} // namespace sabre_pilot::ipc