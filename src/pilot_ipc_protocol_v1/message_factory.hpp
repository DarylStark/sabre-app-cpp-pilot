#pragma once

#include "messages.hpp"
#include <memory>

namespace sabre_pilot::ipc
{
    class MessageFactory
    {
    public:
        std::shared_ptr<PilotIpcMessage> createMessage(uint16_t type);
        std::shared_ptr<ClientHelloMessage> createClientHello();
        std::shared_ptr<ServerHelloMessage> createServerHello();
    };
} // namespace sabre_pilot::ipc