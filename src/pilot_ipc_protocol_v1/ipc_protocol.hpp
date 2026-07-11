#pragma once

#include <memory>
#include <pilot_ipc_protocol/ipc_command.hpp>
#include <pilot_ipc_protocol/ipc_protocol.hpp>

namespace sabre_pilot::ipc
{
    class PilotIpcProtocol_v1 : public sabre_pilot::ipc::PilotIpcProtocol
    {
    public:
        PilotIpcProtocol_v1(
            std::queue<std::unique_ptr<sabre_pilot::ipc::IpcCommand>> &queue);
        void processData();
    };
} // namespace sabre_pilot::ipc