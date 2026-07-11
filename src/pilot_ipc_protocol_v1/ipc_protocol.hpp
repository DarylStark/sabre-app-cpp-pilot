#pragma once

#include "message_factory.hpp"
#include <memory>
#include <pilot_ipc_protocol/ipc_message.hpp>
#include <pilot_ipc_protocol/ipc_protocol.hpp>

namespace sabre_pilot::ipc
{
    class PilotIpcProtocol_v1 : public sabre_pilot::ipc::PilotIpcProtocol
    {
    private:
        MessageFactory _factory;

    public:
        PilotIpcProtocol_v1(
            std::queue<std::shared_ptr<sabre_pilot::ipc::IpcMessage>> &queue);
        void processData();
    };
} // namespace sabre_pilot::ipc