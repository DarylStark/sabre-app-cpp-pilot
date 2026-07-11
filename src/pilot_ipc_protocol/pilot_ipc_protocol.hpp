#pragma once

#include <pilot_core/ipc_protocol.hpp>

namespace sabre_pilot::ipc
{
    class PilotIpcProtocol : public IpcProtocol
    {
    public:
        void processData() override;
    };
} // namespace sabre_pilot::ipc